#include <xc.h>  

#define OUTPUT    0
#define INPUT     1

#define PORTA     0
#define PORTB     1
#define PORTC     2

#define SDO2      0x16
#define SCK2      0x15

#define SDI2_PIN  4
#define SDI2_PORT PORTB
#define SDO2_PIN  5
#define SDO2_PORT PORTB
#define SCK2_PIN  6
#define SCK2_PORT PORTB

#define UP_BUTTON             TRISCBITS.TRISC4
#define DOWN_BUTTON           TRISCBITS.TRISC3
#define LEFT_BUTTON           TRISCBITS.TRISC6
#define RIGHT_BUTTON          TRISCBITS.TRISC7
#define MODE_BUTTON           TRISBBITS.TRISB7

#define UP_Button_Clear()    (IOCCFbits.IOCCF4 = 0)
#define DOWN_Button_Clear()  (IOCCFbits.IOCCF3 = 0)
#define LEFT_Button_Clear()  (IOCCFbits.IOCCF6 = 0)
#define RIGHT_Button_Clear() (IOCCFbits.IOCCF7 = 0)
#define MODE_Button_Clear()  (IOCBFbits.IOCBF7 = 0)

/* Setting negative edge detection bits */
#define UP_BUTTON_NEGEDGE     IOCCNbits.IOCCN4
#define DOWN_BUTTON_NEGEDGE   IOCCNbits.IOCCN3
#define LEFT_BUTTON_NEGEDGE   IOCCNbits.IOCCN6
#define RIGHT_BUTTON_NEGEDGE  IOCCNbits.IOCCN7
#define MODE_BUTTON_NEGEDGE   IOCBNbits.IOCBN7

/* Setting positive edge detection bits */
#define UP_BUTTON_POSEDGE     IOCCPbits.IOCCP4
#define DOWN_BUTTON_POSEDGE   IOCCPbits.IOCCP3
#define LEFT_BUTTON_POSEDGE   IOCCPbits.IOCCP6
#define RIGHT_BUTTON_POSEDGE  IOCCPbits.IOCCP7
#define MODE_BUTTON_POSEDGE   IOCBPbits.IOCBP7
