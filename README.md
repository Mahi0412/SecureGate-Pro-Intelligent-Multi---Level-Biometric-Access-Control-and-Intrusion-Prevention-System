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

The following flowchart illustrates the complete operation of SecureGate Pro, including user authentication, fingerprint verification, administrator operations and automated door control.

```mermaid
flowchart TD
    A([Power ON]) --> B[Initialize LPC2148 and Peripherals]
    B --> C[Initialize UART0, UART1, I2C, LCD and Keypad]
    C --> D[Enable External Interrupt EINT2]
    D --> E[Read Registered User Count from EEPROM]

    E --> F{User Database Empty?}

    F -- Yes --> G[Display NO DATA BASE]
    G --> H[Wait for Administrator Interrupt]
    H --> I[Enter Administrator Menu]

    F -- No --> J[Display Enter 4 Digit User ID]
    J --> K[Enter User ID Through Keypad]
    K --> L{User ID Valid?}

    L -- No --> M[Display INVALID USER ID]
    M --> N[Log ACCESS DENIED Through UART0]
    N --> J

    L -- Yes --> O[Read Stored Password from EEPROM]
    O --> P[Enter 4 Digit Password]
    P --> Q{Password Correct?}

    Q -- No --> R[Display WRONG PASSWORD]
    R --> S[Log Failed Attempt Through UART0]
    S --> P

    Q -- Yes --> T[Start R305 Fingerprint Verification]
    T --> U[Communicate Through UART1]
    U --> V{Fingerprint Verified?}

    V -- No --> W[Access Not Granted]
    W --> J

    V -- Yes --> X[Log ACCESS GRANTED Through UART0]
    X --> Y[Display DOOR OPEN]
    Y --> Z[Activate L293D Motor Driver]
    Z --> AA[Open Door Using DC Motor]
    AA --> AB[Wait for Door Close Switch Input]
    AB --> AC[Activate Motor in Closing Direction]
    AC --> AD[Close Door]
    AD --> AE[Log DOOR CLOSED Through UART0]
    AE --> J

    I --> AF{Select Admin Operation}
    AF -- Edit Password --> AG[Verify Current Password]
    AG --> AH[Enter and Confirm New Password]
    AH --> AI[Update Password in EEPROM]
    AI --> I

    AF -- Fingerprint Management --> AJ[Select Fingerprint Operation]
    AJ --> AK{Select Operation}
    AK -- Enroll --> AL[Enroll Fingerprint Using R305]
    AK -- Search --> AM[Search Fingerprint]
    AK -- Delete --> AN[Delete Selected Fingerprint]
    AK -- Delete All --> AO[Delete All Fingerprints]

    AL --> I
    AM --> I
    AN --> I
    AO --> I

    AF -- Exit --> AP([Return to Main Access Loop])

    classDef startEnd fill:#d5f5e3,stroke:#229954,color:#145a32,stroke-width:2px;
    classDef process fill:#dbeafe,stroke:#2563eb,color:#1e3a8a,stroke-width:1.5px;
    classDef decision fill:#fff3cd,stroke:#d39e00,color:#664d03,stroke-width:1.5px;
    classDef error fill:#fce4e4,stroke:#dc3545,color:#842029,stroke-width:1.5px;

    class A,AP startEnd;
    class B,C,D,E,G,H,I,J,K,O,P,T,U,X,Y,Z,AA,AB,AC,AD,AE,AG,AH,AI,AJ,AL,AM,AN,AO process;
    class F,L,Q,V,AF,AK decision;
    class M,N,R,S,W error;


