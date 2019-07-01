#include <xc.h>
#include "rtcc.h"

/* Macro definitions */
#define OUTPUT     0
#define INPUT      1
#define PORTA      0
#define PORTB      1
#define PORTC      2
#define SDO2       0x16
#define SCK2       0x15
#define SDI2_PIN   4
#define SDI2_PORT  PORTB
#define SDO2_PIN   5
#define SDO2_PORT  PORTB
#define SCK2_PIN   6
#define SCK2_PORT  PORTB
#define DATESET    0
#define WKDAYSET   1
#define TIMESET    2
#define TIMEDISP   3
#define DATEDISP   4
#define DAYSEL     0
#define MONSEL     1
#define YRSEL      2
#define WKDSEL     3
#define HRSEL      4
#define MINSEL     5
#define SECSEL     6
#define BCD_TO_DEC 0
#define DEC_TO_BCD 1
#define DELAY_TICK (_XTAL_FREQ / 2)

/* Button port and pin locations */
#define UP_BUTTON            TRISCBITS.TRISC4
#define DOWN_BUTTON          TRISCBITS.TRISC3
#define LEFT_BUTTON          TRISCBITS.TRISC6
#define RIGHT_BUTTON         TRISCBITS.TRISC7
#define MODE_BUTTON          TRISBBITS.TRISB7

/* Negative edge detection bits */
#define UP_BUTTON_NEGEDGE    IOCCNbits.IOCCN4
#define DOWN_BUTTON_NEGEDGE  IOCCNbits.IOCCN3
#define LEFT_BUTTON_NEGEDGE  IOCCNbits.IOCCN6
#define RIGHT_BUTTON_NEGEDGE IOCCNbits.IOCCN7
#define MODE_BUTTON_NEGEDGE  IOCBNbits.IOCBN7

/* Positive edge detection bits */
#define UP_BUTTON_POSEDGE    IOCCPbits.IOCCP4
#define DOWN_BUTTON_POSEDGE  IOCCPbits.IOCCP3
#define LEFT_BUTTON_POSEDGE  IOCCPbits.IOCCP6
#define RIGHT_BUTTON_POSEDGE IOCCPbits.IOCCP7
#define MODE_BUTTON_POSEDGE  IOCBPbits.IOCBP7

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

extern volatile uint8_t mode = DATESEL;
extern volatile uint8_t digit = 0;
extern volatile char num[7];
extern volatile char maxval[7] = {31, 12, 99, 7, 23, 59, 59};
extern volatile char minval[7] = {1, 1, 0, 1, 0, 0, 0};

void GPIO_Init(void);
void Interrupt_Init(void);
uint8_t convert_BCD(uint8_t, bool);
void set_nixie(void);
void nixie_toggle(void);
void down_button_ISR(void);
void up_button_ISR(void);
void left_button_ISR(void);
void right_button_ISR(void);
void mode_button_ISR(void);
void interrupt Interrupt_Handler(void);
