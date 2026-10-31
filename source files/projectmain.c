#include<LPC21XX.h>
#include"defines.h"
#include"types.h"
#include"delay.h"
#include"lcd.h"
#include"lcd_defines.h"
#include"kpm.h"
#include "ext_int.h"
#include "i2c.h"
#include "i2c_eeprom.h"
#include "device.h"
#include "R305.h" 
#include "uart0.h"
#include "uart1.h"

#include<string.h>
#define SW_AL 5
unsigned char pass[5],curpass[5],newpass[5];// Buffers used for storing the current, existing and newly entered passwords.
unsigned char *pas=pass,*curpas=curpass,*newpas=newpass;
unsigned char choose,k=0;// Variables used by the administrator menu and fingerprint-template operations.
unsigned int pageid;
int usrcnt=0,rusrcnt;// User count variables: usrcnt is used while updating the count; rusrcnt is read from EEPROM.
u8 userid[5],ruserid[5],pwd[5],rpwd[5];
u8 usrid[5],rpd[5],pd[5];
u32 count=0;// Counts password failures; also used in the account-lock message condition.
void menu(void);
void edit_pass(void);
void edit_fp(void);
extern unsigned char a;// 'a' is set by the external-interrupt module to request the administrator menu.
int flag=0;

int mystrcmp(const char* a,const char* b)
{
	int k;
	for(k=0;a[k];k++)
	{
		if(a[k]!=b[k])
		{
			return a[k]-b[k];
		}
	}
	return 0;	
}


int main()
{
	int usrcheck;
	InitUART1();// Initialize UART1 for communication with the R305 fingerprint module.
	InitUART0();// Initialize UART0 for sending system and access-log messages to the PC.
	UART0_Str("********************************************************\r\n");
	UART0_Str("*                     WELCOME TO                       *\r\n");
	UART0_Str("*             SECUREGATE PRO - INTELLIGENT             *\r\n");
	UART0_Str("*                MULTI-LEVEL BIOMETRIC                 *\r\n");
	UART0_Str("*                 ACCESS CONTROL AND                   *\r\n");
	UART0_Str("*                INTRUSION PREVENTION                  *\r\n");
	UART0_Str("*                      SYSTEM                          *\r\n");
	UART0_Str("********************************************************\r\n");
	UART0_Str("*******************SYSTEM STARTED***********************\r\n");
	// Initialize the I2C interface, LCD and matrix keypad.
	init_i2c();
	InitLCD();
	Init_KPM();
	UART0_Str("*****************SYSTEM INITIALISED*********************\r\n");
	UART0_Str("********************************************************\r\n");
	// Enable the configured external interrupt used to request the administrator menu.
	Enable_EINT2();
	// Read the registered-user count from EEPROM address 0x0000.
	//i2c_eeprom_bytewrite(0x50,0x0000,usrcnt);
	rusrcnt=i2c_eeprom_randomread(0x50,0x0000);
	UART0_Str("Rusrcnt: ");
	UART0_Int(rusrcnt);
	
	//i2c_eeprom_pagewrite(0x50,0x0100,"1111",4);
	//i2c_eeprom_seqread(0x50,0x0100,4,rpwd,4);
	while(1)
	{
		LABEL1:CmdLCD(CLEAR_LCD);
		// If EEPROM reports no registered users, display the database message and wait for the admin request.
		if(rusrcnt==0)
		{
			CmdLCD(0x01);
			StrLCD("NO DATA BASE");
			UART0_Str("*                     NO DATA BASE                     *\r\n");
			while(a==0);
			a=0;
			menu();
		}
		else
		{
		// Ask the user to enter the four-digit User ID through the keypad
		CmdLCD(0x01);
		StrLCD("Enter 4 digit id:");
		while(ColScan()==1)
		{
			// If the external-interrupt request flag is set, open the administrator menu.
			if(a==1)
			{
				a=0;
				menu();
				//goto LABEL1;
			}
		}
		// Move the LCD cursor to the second line, clear the ID buffer and read the entered ID.
		CmdLCD(GOTO_LINE2_POS0);
		memset(userid,'\0',5);
		Read(userid);

        // Search the EEPROM user records for the entered User ID.
		for(k=0;k<rusrcnt;k++)
		{
			i2c_eeprom_seqread(0x50,(0x0002*(k*5)),ruserid,4);
			// Compare the entered ID buffer with the ID read from the current EEPROM record.
			if((mystrcmp((const char*)usrid,(const char*)ruserid))==0)
			{
				// Record successful User ID verification in the PC access log.
				U0_LogUser((char *)usrid, "USER ID VERIFIED");
				flag=1;
			}
			// Continue to password verification when the ID comparison succeeds.
			if(flag==1)
			{
				flag=0;
				// Read the stored password from EEPROM.
				i2c_eeprom_seqread(0x50,0x0100,rpwd,4);
PASS:			CmdLCD(CLEAR_LCD);
				StrLCD("Enter 4 digit pwd:");// Ask the user to enter the four-digit password.
				CmdLCD(GOTO_LINE2_POS0);
				Read(pd);
				CmdLCD(GOTO_LINE2_POS0);// Display the entered password buffer briefly on the LCD.
				StrLCD(pd);
				delay_ms(1000);
				// Compare the entered password with the password read from EEPROM.
				if((mystrcmp((const char*)pd,(const char*)rpwd))==0)
	 			{
					U0_LogUser((char *)usrid, "PASSWORD VERIFIED");
					CmdLCD(0x01);
	   				CmdLCD(0x80);
					// Start fingerprint verification using the R305 module.
          			if(search_fp())
					{
						// Log fingerprint verification and grant access only after a successful search.
				   		U0_LogUser((char *)usrid, "FINGERPRINT VERIFIED");
				   		U0_LogUser((char *)usrid, "ACCESS GRANTED");
						CmdLCD(CLEAR_LCD);// Display the door status and rotate the motor in the opening direction.
						CmdLCD(GOTO_LINE1_POS0);
						StrLCD("DOOR OPEN\r\n");
						motor_open();
						U0_LogUser((char *)usrid, "DOOR OPENED");
						U0_LogUser((char *)usrid, "PRESS SW TO CLOSE");
						while(READBIT(IOPIN0,SW_AL)==1); // Wait until the configured switch input becomes low, then close the door.
                    	motor_close();
						U0_LogUser((char *)usrid, "DOOR CLOSED");
					} 
					delay_ms(1000);
				}
				else
				{
					// Handle an incorrect password and update the failed-attempt counter.
					CmdLCD(CLEAR_LCD);
					CmdLCD(GOTO_LINE1_POS0);
					if(count==3)
					{
						U0_LogUser((char *)usrid, "ACCOUNT LOCKED ");
						U0_LogUser((char *)usrid, "ACCESS DENIED ");
					}
					count++;
					StrLCD("pwd incorrect");
					U0_LogUser((char *)usrid, "WRONG PASSWORD ");
				//U0_LogUser((char *)usrid, "ACCESS DENIED ");
                CmdLCD(GOTO_LINE2_POS0);
				goto PASS;
				/*StrLCD("Wait & Enter agn");
				delay_s(1);*/
			}
		}
		else
		{
			// Display and log an invalid User ID when no matching record is found.
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD("Usr id incorrect");
			U0_LogUser((char *)usrid, "INVALID USER ID");
			U0_LogUser((char *)usrid, "ACCESS DENIED");
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("Wait & Enter agn");
			delay_s(1);
		}
		}
	}
	}
}

// Administrator menu: provides password editing, fingerprint management and exit options.
void menu()
{
	//InitUART1();
	char ch;
	a=0;
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	UART0_Str("*****************ADMIN - LOGIN SUCCESSFUL****************\r\n");
	while(1)
	{
		StrLCD("MENU: ");
		StrLCD("1)Edit pass");
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("2)Edit FP 3)EXIT");
		ch=KeyScan();// Read the administrator's menu selection from the keypad.
		switch(ch)
		{
			case '1':edit_pass();
					break;
			case '2':edit_fp();
					break;
			case '3':CmdLCD(0x01);
					return;
		}
		
	}
}
// Fingerprint administration: supports enrolment, searching, deleting one template and deleting all templates.
void edit_fp(void)
{
    //InitLCD();
    //InitUART1();
	CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD("FINGER PRINT");
	UART0_Str("***********************FINGERPRINT**********************\r\n");

    delay_ms(2000);

    while (1)
    {
		// Display the fingerprint-management options.
	   	CmdLCD(0x01);
	   	CmdLCD(0x80);
	   	StrLCD("1.ENROLL 3.DEL ");		
		CmdLCD(0xc0);
		StrLCD("2.SEARCH 4.D.ALL");
		choose = KeyScan();
		while(ColScan()==0);
		//delay_ms(100);
		switch(choose)
		{
		
		case '1':
		// Enrol a fingerprint and associate the entered user information with the selected template ID.
			CmdLCD(0x01);
	   		CmdLCD(0x80);
			StrLCD("Enter user ID:");
			CmdLCD(0xC0);
			Read(usrid);
	   		delay_ms(1000);
			CmdLCD(0x01);
			StrLCD("Enter ID: ");
			pageid = ReadNum();
	   		CmdLCD(0xC0);
	   		u32LCD(pageid);				
    		delay_ms(100);
			// Accept template IDs in the range checked by this condition and call the R305 enrolment routine.
			if(pageid>0 && pageid <10)
			{
				if((enroll(pageid)))
				{
					// Log successful enrolment, store the User ID in EEPROM and update the registered-user count.
					UART0_Str("LOG: USER ID: ");
					UART0_Str(usrid);
					UART0_Str("  FINGERPRINT ENROLLED\r\n");
					i2c_eeprom_pagewrite(0x50,(0x0002*(usrcnt*5)),usrid,4);
					usrcnt++;
					i2c_eeprom_bytewrite(0x50,0x0000,usrcnt);
					rusrcnt=i2c_eeprom_randomread(0x50,0x0000);
					UART0_Int(rusrcnt);
					UART0_Str("\r\n");
				}
			}
			break;
	
		case '2':
		// Search the R305 fingerprint library and display the returned template/search result.
	   		CmdLCD(0x01);
	   		CmdLCD(0x80);
	   		StrLCD("PLACE FINGER");
         	CmdLCD(0xC0);
          	u32LCD(search_fp()); 
			U0_LogUser((char *)usrid, "SEARCH SUCCESSFUL ");
			delay_ms(1000); 
			break;

		case '3':
		// Delete the selected fingerprint template when the entered template ID is within the accepted range
			CmdLCD(0x01);
	   		CmdLCD(0x80);
	   		StrLCD("Enter ID: ");
			pageid = ReadNum();
	   		CmdLCD(0xC0);
	   		u32LCD(pageid);					
    		delay_ms(100);
			if(pageid>0 && pageid <10)
			{
				if(delete_fp(pageid) == 0x00)
				{
					// Update the user count in EEPROM after successful fingerprint deletion.
					CmdLCD(0x01);
			   		CmdLCD(0x80);
			   		StrLCD("SELECTED ID DEL.");	
					U0_LogUser((char *)usrid, "SELECTED ID DEL ");
					if(usrcnt>0)
					usrcnt--;
					i2c_eeprom_bytewrite(0x50,0x0000,usrcnt);
					rusrcnt=i2c_eeprom_randomread(0x50,0x0000);				
					delay_ms(1000);
				}
				else
				{
					// Display and log the failure when the fingerprint deletion command is unsuccessful.
					CmdLCD(0x01);
			   		CmdLCD(0x80);
			   		StrLCD("FAILED TO DEL");
					U0_LogUser((char *)usrid, "FAILED TO DEL ");					
					delay_ms(1000);
				}
			}
			break;
		case '4':
			if(deleteall_fp() == 0x00)
			{
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("DEL ALL SUCCUSS");					
				delay_ms(1000);	
			}
			else
			{
				CmdLCD(0x01);
			   	CmdLCD(0x80);
			   	StrLCD("FAILED DEL.");					
				delay_ms(1000);				
			}
			break;
		default: return;
		}
    }
}
// Password-management routine: verifies the current password and asks for matching new-password entries.
void edit_pass(void)
{
	i2c_eeprom_pagewrite(0x50,0x0100,"1111",4);
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("Ent 4 dig cur Pass:");
	CmdLCD(GOTO_LINE2_POS0);
	Read(pas);// Read the current password entered by the administrator.
	i2c_eeprom_seqread(0x50,0x0100,curpass,4);// Read the currently stored password from EEPROM for comparison.
	if(!(strcmp((const char*)pas,(const char*)curpas)))
	{

		// Ask for a new password after the current password has been accepted
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("Ent 4 dig New pas:");
		CmdLCD(GOTO_LINE2_POS0);
		Read(pas);
		CmdLCD(CLEAR_LCD);// Ask the administrator to re-enter the new password for confirmation.
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("Ent 4 dig New pas agn:");
		CmdLCD(GOTO_LINE2_POS0);
		Read(newpas);
		// Update EEPROM only when the new password and confirmation match.
		if(!(strcmp((const char*)pas,(const char*)newpas)))
		{
			i2c_eeprom_pagewrite(0x50,0x0100,newpas,4);
			CmdLCD(CLEAR_LCD);
			StrLCD("PASS CHANGE SUC");
			U0_LogUser((char *)usrid, "PASSWORD UPDATED ");
			delay_s(1);
			return;
		}
		else
		{
			// Display failure when the two new-password entries do not match.
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD("PWD CHANGE FAIL");
			delay_ms(300);
			return;
		}
	}
	else
	{
		// Display failure when the entered current password does not match the EEPROM value.
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("PWD CHANGE FAIL");
		delay_ms(200);
		return;
	}
}
