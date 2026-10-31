# SecureGate-Pro-Intelligent-Multi---Level-Biometric-Access-Control-and-Intrusion-Prevention-System
SecureGate Pro is an LPC2148-based embedded access-control system featuring three-level authentication using User ID, password and R305 fingerprint verification. It integrates EEPROM storage, UART communication, LCD feedback and L293D motor control for secure, automated door access.

### Overview

SecureGate Pro is an LPC2148 ARM7-based embedded access-control system developed using Embedded C. It provides three-level user authentication through User ID, password and fingerprint verification using the R305 fingerprint module. The system uses an AT24C256 EEPROM for user-related data storage, a 4×4 matrix keypad for input, a 16×2 LCD for status display and an L293D motor driver to control a DC motor for door operation.

The project also demonstrates UART-based communication, I²C interfacing, external interrupt handling, biometric sensor integration, EEPROM operations and modular Embedded C programming.

### Objective

To design and implement an LPC2148-based intelligent access-control system that restricts unauthorized access through User ID, password and fingerprint authentication. The system grants access only after successful verification and controls a motor-driven door mechanism while reporting system and access events to a PC through UART0.

### Features

- **Three-Level Authentication**: User ID, password and R305 fingerprint verification.

- **Fingerprint Management**: Supports fingerprint enrolment, search and deletion through the administrator menu.

- **EEPROM Data Storage**: Uses AT24C256 EEPROM over I²C for storing user-related information.

- **Password Management**: Provides an administrator option to update the password.

- **Keypad Interface**: Uses a 4×4 matrix keypad for User ID, password and menu selection.

- **LCD Status Display**: Displays authentication prompts, results and door status.

- **Automated Door Control**: Uses an L293D motor driver to operate the DC motor for opening and closing the door.

- **UART0 Event Logging**: Sends system and user-access events to a PC serial terminal.

- **UART1 Fingerprint Communication**: Communicates with the R305 fingerprint module.

- **External Interrupt**: Provides an interrupt-based request to enter the administrator menu.

### Hardware Requirements

| S.No. | Hardware Component | Specification / Model | Purpose |
|:---:|---|---|---|
| 1 | Microcontroller | LPC2148 ARM7 | Main controller for authentication and system operations |
| 2 | Fingerprint Sensor | R305 | Fingerprint enrolment and biometric verification |
| 3 | EEPROM | AT24C256 | Non-volatile storage of user credentials and data |
| 4 | LCD Display | 16×2 LCD | Displays authentication prompts and system status |
| 5 | Matrix Keypad | 4×4 Keypad | User ID, password and menu input |
| 6 | Motor Driver | L293D | Controls the DC motor direction |
| 7 | DC Motor | DC Geared Motor | Represents door opening and closing |
| 8 | Switches | Push Button Switches | Administrator interrupt and door control |
| 9 | Communication Interface | USB-to-UART / MAX232 | Serial communication with PC |
| 10 | Power Supply | Regulated 5V DC | Supplies power to the development board and compatible modules |
| 11 | Connecting Wires | Jumper Wires | Hardware interconnections |

Check the voltage requirements of each module and use a common ground where required. The motor must be powered through the motor driver, not directly from an LPC2148 GPIO pin.

### System Block Diagram

The LPC2148 acts as the central controller and coordinates the keypad, LCD, EEPROM, fingerprint module and motor-control unit. UART0 is used for PC-side logging, while UART1 is used for communication with the R305 fingerprint module.
<img width="1754" height="896" alt="image" src="https://github.com/user-attachments/assets/bcb42801-8854-4118-835c-41563e8c9417" />

## Pin Configuration

### LPC2148 Pin Mapping

| S.No. | Peripheral | Signal | LPC2148 Pin | Connection / Description |
|:---:|---|---|---|---|
| 1 | UART0 | TXD0 | P0.0 | PC serial log transmission |
| 2 | UART0 | RXD0 | P0.1 | PC serial data reception |
| 3 | UART1 (R305) | TXD1 | P0.8 | Connected to R305 RX |
| 4 | UART1 (R305) | RXD1 | P0.9 | Connected to R305 TX |
| 5 | I2C (EEPROM) | SCL | P0.2 | EEPROM clock |
| 6 | I2C (EEPROM) | SDA | P0.3 | EEPROM data |
| 7 | LCD | RS | P0.12 | Register Select |
| 8 | LCD | RW | P0.13 | Read/Write control |
| 9 | LCD | Data D0–D7 | P0.16–P0.23 | 8-bit data bus |
| 10 | Keypad | Rows | P1.16–P1.19 | 4×4 keypad row connections |
| 11 | Keypad | Columns | P1.20–P1.23 | 4×4 keypad column connections |
| 12 | L293D | 1A | P1.24 | Motor direction input 1 |
| 13 | L293D | 2A | P1.25 | Motor direction input 2 |
| 14 | L293D | 1,2EN | P1.28 | Motor driver enable |
| 15 | Door Control | Switch Input | P0.5 | Door closing switch |
| 16 | External Interrupt | EINT2 | P0.7 | Administrator menu request |

## System Architecture

The SecureGate Pro system follows a modular architecture in which the LPC2148 ARM7 microcontroller acts as the central controller, coordinating authentication, data storage, user input, communication and door control.

### 1. Processing Unit

* **LPC2148 ARM7 Microcontroller:** Acts as the central controller and manages the authentication sequence, peripheral communication, administrator operations and door control.

### 2. Input Unit

* **4×4 Matrix Keypad:** Accepts user IDs, passwords and administrator commands.
* **External Interrupt (EINT2 – P0.7):** Provides access to the administrator menu.

### 3. Authentication Unit

* **R305 Fingerprint Sensor:** Performs fingerprint enrollment and verification through UART1 communication.

### 4. Memory Unit

* **AT24C256 EEPROM:** Stores registered user information and password data through the I²C communication interface.

### 5. Display Unit

* **16×2 LCD:** Displays authentication prompts, system messages, access status and door operation updates.

### 6. Door Control Unit

* **L293D Motor Driver:** Controls the DC motor according to authentication results and door operation commands.
* **DC Motor:** Performs the door opening and closing operations.

### 7. Communication and Logging Unit

* **UART0:** Establishes serial communication with a PC for system monitoring and access-event logging.
* **UART1:** Handles communication between the LPC2148 and R305 fingerprint sensor.
* **I²C:** Enables communication between the LPC2148 and AT24C256 EEPROM.


### System Working

### 1. System Initialization

When the system is powered ON, the LPC2148 initializes UART0, UART1, I²C, LCD, keypad and the configured external interrupt. UART0 sends the system-start and initialization messages to the PC serial terminal.

### 2. EEPROM User-Count Check

The controller reads the registered-user count from the AT24C256 EEPROM. If no user records are available, the LCD displays NO DATA BASE and the system waits for the administrator request.

### 3. Administrator Operations

The configured external interrupt requests entry to the administrator menu. The administrator can select the available operations:

-Edit password

-Fingerprint management

-Exit the menu

The fingerprint-management menu provides options for enrolment, search, deletion of a selected template and deletion of all templates.

### 4. User ID Verification

During normal access, the user enters a four-digit User ID through the keypad. The controller checks the entered ID against the user records stored in EEPROM. If the ID is not valid, the LCD displays an error and UART0 logs the invalid User ID and access-denied events.

### 5. Password Verification

After successful User ID verification, the system requests the four-digit password. The entered password is compared with the password stored in EEPROM. An incorrect password is reported on the LCD and through UART0. A successful password check allows the process to continue to fingerprint verification.

### 6. Fingerprint Verification

The R305 fingerprint module communicates with the LPC2148 through UART1. The user places a finger on the sensor, and the system performs the fingerprint search. If the fingerprint search succeeds, the system proceeds to grant access.

### 7. Access Granted and Door Opening

After successful User ID, password and fingerprint verification, UART0 records the authentication result and access-granted event. The LCD displays DOOR OPEN, and the LPC2148 activates the L293D motor driver to operate the DC motor in the opening direction.

### 8. Door Closing

The system waits for the configured switch input. When the close condition is detected, the controller operates the motor in the closing direction and sends the door-closed event through UART0.

## Project Workflow

The following flowchart illustrates the overall working process of SecureGate Pro, including multi-level authentication, administrator operations and automated door control.
```                  +------------------+
                  |     POWER ON     |
                  +--------+---------+
                           |
                           v
              +--------------------------+
              | Initialize LPC2148 and   |
              | Required Peripherals     |
              +------------+-------------+
                           |
                           v
              +--------------------------+
              | Read User Count from     |
              | AT24C256 EEPROM          |
              +------------+-------------+
                           |
                           v
                  +------------------+
                  | User Database    |
                  | Available?       |
                  +----+---------+---+
                       |         |
                  NO   |         | YES
                       |         |
                       v         v
             +----------------+  +------------------+
             | Display NO     |  | Enter 4-Digit    |
             | DATA BASE      |  | User ID          |
             +-------+--------+  +--------+---------+
                     |                    |
                     |                    v
                     |           +------------------+
                     |           | Verify User ID   |
                     |           +--------+---------+
                     |                    |
                     |                    v
                     |           +------------------+
                     |           | User ID Valid?   |
                     |           +----+---------+---+
                     |                |         |
                     |           NO   |         | YES
                     |                |         |
                     |                v         v
                     |       +-------------+  +------------------+
                     |       | Access       |  | Enter 4-Digit    |
                     |       | Denied       |  | Password         |
                     |       +------+------+  +--------+---------+
                     |              |                  |
                     |              |                  v
                     |              |         +------------------+
                     |              |         | Verify Password  |
                     |              |         +--------+---------+
                     |              |                  |
                     |              |                  v
                     |              |         +------------------+
                     |              |         | Password Correct?|
                     |              |         +----+---------+---+
                     |              |              |         |
                     |              |         NO   |         | YES
                     |              |              |         |
                     |              |              v         v
                     |              |      +-------------+  +------------------+
                     |              |      | Wrong       |  | Fingerprint      |
                     |              |      | Password    |  | Verification     |
                     |              |      +------+------+  | Using R305       |
                     |              |             |         +--------+---------+
                     |              |             |                  |
                     |              |             |                  v
                     |              |             |         +------------------+
                     |              |             |         | Fingerprint     |
                     |              |             |         | Matched?         |
                     |              |             |         +----+---------+---+
                     |              |             |              |         |
                     |              |             |         NO   |         | YES
                     |              |             |              |         |
                     |              |             |              v         v
                     |              |             |       +----------+  +----------------+
                     |              |             |       | Access   |  | ACCESS GRANTED |
                     |              |             |       | Denied   |  +-------+--------+
                     |              |             |       +----+-----+          |
                     |              |             |            |                v
                     |              |             |            |       +----------------+
                     |              |             |            |       | OPEN DOOR      |
                     |              |             |            |       | L293D + DC     |
                     |              |             |            |       | MOTOR          |
                     |              |             |            |       +-------+--------+
                     |              |             |            |               |
                     |              |             |            |               v
                     |              |             |            |       +----------------+
                     |              |             |            |       | DOOR CLOSING   |
                     |              |             |            |       | OPERATION      |
                     |              |             |            |       +-------+--------+
                     |              |             |            |               |
                     +--------------+-------------+------------+---------------+
                                                    |
                                                    v
                                      +--------------------------+
                                      | Return to Main Access    |
                                      | Control Loop             |
                                      +--------------------------+


ADMINISTRATOR OPERATIONS:

       External Interrupt (EINT2 - P0.7)
                       |
                       v
              +--------------------+
              | Administrator Menu |
              +---------+----------+
                        |
                        v
              +--------------------+
              | Select Operation   |
              +----+----------+----+
                   |          |
          Edit Password      Fingerprint Management
                   |          |
                   v          v
          Update Password   +----------------------+
          in EEPROM         | Enroll / Search /    |
                            | Delete / Delete All   |
                            +----------+-----------+
                                       |
                                       v
                              Return to Admin Menu
```
### Communication Interfaces

* **UART0:** PC communication and access-event logging.
* **UART1:** R305 fingerprint module communication.
* **I²C:** AT24C256 EEPROM communication.
  
### Development Tools & Environment

1.**Microcontroller**: LPC2148 ARM7

2.**Programming Language**: Embedded C

3.**IDE**: Keil µVision

4.**Programming Tool**: Flash Magic

5.**Communication Interfaces**: UART0, UART1 and I²C

6.**Core Concepts**:GPIO, EEPROM interfacing, external interrupts, keypad, LCD interfacing, fingerprint sensor communication and motor control.

## Future Enhancements
1. Implement fingerprint liveness detection to prevent fake fingerprint access.
2. Integrate RFID cards for additional authentication.
3. Introduce a duress password for emergency access alerts.
4. Restrict user access based on predefined time schedules.
5. Enable automatic door locking after a predefined timeout.
6. Integrate IoT connectivity for remote access monitoring and security alerts.
7. Maintain timestamp-based access logs to track successful and failed authentication attempts for security auditing.

### Applications

1. **Smart Residential Security:** Provide biometric-based door access for apartments, independent houses and gated communities.
2. **Bank Vault Security:** Protect cash vaults, lockers and areas containing valuable financial assets through multi-level authentication.
3. **Hospitals and Healthcare Facilities:** Restrict access to medicine storage rooms, pharmacies and confidential patient-record sections.
4. **Educational Institutions:** Secure examination rooms, laboratories, staff rooms and administrative departments from unauthorized access.
5. **Warehouses and Inventory Storage:** Monitor and control access to storage facilities containing valuable goods, electronic components and sensitive materials.

### Developed By

Tekula Srimahi

**Project**: SecureGate Pro – Intelligent Multi-Level Biometric Access Control and Intrusion Prevention System



