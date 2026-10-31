//uart1.c
#include <LPC21xx.H>  /* LPC21xx definitions         */

#include "uart1.h"
#include "lcd.h"
#include "lcd_defines.h"
char rec_buff[25],dummy;
unsigned char i=0,ch,r_flag;

extern unsigned char buf[25]; 

void UART1_isr(void) __irq
{
  
  if((U1IIR & 0x04)) //check if receive interrupt
  {
		//StrLCD()
		ch = U1RBR;	/* Read to Clear Receive Interrupt */
		if(i<25)
			rec_buff[i++] = ch; 
  }
  else
  {
      dummy=U1IIR; //Read to Clear transmit interrupt
  
  }
   VICVectAddr = 0; /* dummy write */
}

void InitUART1 (void) /* Initialize Serial Interface       */ 
{  
            		
  PINSEL0 |= 0x00050000;  /* Enable UART1 TXD1 and RXD1 pin functions */
  U1LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */
  U1DLL = DIVISOR;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */
  U1DLM = DIVISOR>>8;
  U1LCR = 0x03;         /* DLAB = 0  */
  
  #if UART_INT_ENABLE > 0

  VICIntSelect = 0x00000000; // IRQ
  VICVectAddr0 = (unsigned)UART1_isr;
  VICVectCntl0 = 0x20 | 7; /* Enable UART1 interrupt in VIC slot 0 */
  VICIntEnable = 1 << 7;  /* Enable UART1 interrupt */
 
  U1IER = 0x03;       /* Enable UART1  RX and THRE Interrupts */   
             
  #endif
						
}

void UART1_Tx(char ch)  /* Write character to Serial Port    */  
{ 
  while (!(U1LSR & 0x20));
  U1THR = ch;                
}

char UART1_Rx(void)    /* Read character from Serial Port   */
{                     
  while (!(U1LSR & 0x01));
  return (U1RBR);
}

void UART1_Str(char *s)
{
   while(*s)
       UART1_Tx(*s++);
}

void UART1_Str1(unsigned char n)
{
 	unsigned char j;   
    for(j=0;j<n;j++)
    {
        UART1_Tx(buf[j]);
    }   
}

void UART1_Int(unsigned int n)
{
  unsigned char a[10]={0,0,0,0,0,0,0,0,0,0};
  int i=0;
  if(n==0)
  {
    UART1_Tx('0');
		return;
  }
  else
  {
     while(n>0)
	 {
	   a[i++]=(n%10)+48;
	   n=n/10;
	 }
	 --i;
	 for(;i>=0;i--)
	 {
	   UART1_Tx(a[i]);
	 }
   }
}

void UART1_Float(float f)
{
  int x;
  float temp;
  x=f;
  UART1_Int(x);
  UART1_Tx('.');
  temp=(f-x)*100;
  x=temp;
  UART1_Int(x);
}


