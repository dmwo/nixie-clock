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
#define DATESEL
#define TIMESEL
#define TIME
#define DATE

/* Button port and pin locations */
#define UP_BUTTON             TRISCBITS.TRISC4
#define DOWN_BUTTON           TRISCBITS.TRISC3
#define LEFT_BUTTON           TRISCBITS.TRISC6
#define RIGHT_BUTTON          TRISCBITS.TRISC7
#define MODE_BUTTON           TRISBBITS.TRISB7

/* Negative edge detection bits */
#define UP_BUTTON_NEGEDGE     IOCCNbits.IOCCN4
#define DOWN_BUTTON_NEGEDGE   IOCCNbits.IOCCN3
#define LEFT_BUTTON_NEGEDGE   IOCCNbits.IOCCN6
#define RIGHT_BUTTON_NEGEDGE  IOCCNbits.IOCCN7
#define MODE_BUTTON_NEGEDGE   IOCBNbits.IOCBN7

/* Positive edge detection bits */
#define UP_BUTTON_POSEDGE     IOCCPbits.IOCCP4
#define DOWN_BUTTON_POSEDGE   IOCCPbits.IOCCP3
#define LEFT_BUTTON_POSEDGE   IOCCPbits.IOCCP6
#define RIGHT_BUTTON_POSEDGE  IOCCPbits.IOCCP7
#define MODE_BUTTON_POSEDGE   IOCBPbits.IOCBP7

/* GPIO Operation Aliases */
#define Global_Int_Enable()     (INTCONbits.GIE = 1)
#define Peripheral_Int_Enable() (INTCONbits.PEIE = 1)
#define Nested_Int_Disable()    (INTCONbits.NSTDIS = 1)
#define IOC_Int_Enable()        (PIE0bits.IOCIE = 1)
#define Timer_Int_Enable()      (PIE0bits.TMR0IE = 1)
#define Up_Int_Clear()          (IOCCFbits.IOCCF4 = 0)
#define Down_Int_Clear()        (IOCCFbits.IOCCF3 = 0)
#define Left_Int_Clear()        (IOCCFbits.IOCCF6 = 0)
#define Right_Int_Clear()       (IOCCFbits.IOCCF7 = 0)
#define Mode_Int_Clear()        (IOCBFbits.IOCBF7 = 0)

typedef struct {
    uint8_t mode = DATESEL;
    uint8_t digit = 0;
    uint8_t num1 = 0;
    uint8_t num2 = 0;
    uint8_t num3 = 0;
} Param_t;

extern volatile Param_t init;
extern volatile uint8_t mode = DATESEL;
extern volatile uint8_t digit = 0;
extern volatile char num[3] = 0;

void GPIO_Init(void);
