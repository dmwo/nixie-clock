#include <xc.h>
#include "pic16f18445.h"
#include "rtcc.h"

/******************************************************************************
 * filename: gpio.h                                                           *
 *                                                                            *
 * purpose: Contains functions pertaining to the operation of the 
 *                                                                            *
 * date created:
 *                                                                            *
 * authors: Dylan Oh                                                          *
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS                                                          *
 *****************************************************************************/

#define OUTPUT     0
#define INPUT      1
#define BCD_TO_DEC 0
#define DEC_TO_BCD 1
#define DELAY_TICK (_XTAL_FREQ / 2)

/* Clock mode macros */
#define DATESET    0
#define WKDAYSET   1
#define TIMESET    2
#define TIMEDISP   3
#define DATEDISP   4

/* Clock setting macros */
#define DAYSEL     0
#define MONSEL     1
#define YRSEL      2
#define WKDSEL     3
#define HRSEL      4
#define MINSEL     5
#define SECSEL     6

/* Serial communication macros */
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

/* Port registers for switches */
#define SW_HRDAY  PORTAbits.RA0
#define SW_MINMON PORTBbits.RB4
#define SW_SECYR  PORTBbits.RB5
#define SW_HV     PORTCbits.RC0 //move to timer

/* Direction registers for buttons and switches */
#define DIR_BUTTON_UP        TRISCbits.TRISC4
#define DIR_BUTTON_DOWN      TRISCbits.TRISC3
#define DIR_BUTTON_LEFT      TRISCbits.TRISC6
#define DIR_BUTTON_RIGHT     TRISCbits.TRISC7
#define DIR_BUTTON_MODE      TRISBbits.TRISB7

#define DIR_SW_HRDAY         TRISAbits.TRISA0
#define DIR_SW_MINMON        TRISBbits.TRISB4
#define DIR_SW_SECYR         TRISBbits.TRISB5
#define DIR_SW_HV            TRISCbits.TRISC0

#define DIR_SDO2             TRISBbits.TRISB5
#define DIR_SDI2             TRISBbits.TRISB4
#define DIR_SCK2             TRISBbits.TRISB6

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

extern volatile uint8_t mode = DAYSEL;
extern volatile uint8_t digit = 0;
extern volatile char num[7];
extern volatile char maxval[7] = {31, 12, 99, 7, 23, 59, 59};
extern volatile char minval[7] = {1, 1, 0, 1, 0, 0, 0};

/*
 * function name: GPIO_Init
 *
 * description: Initialises the GPIO pins:
 *                  - I2C SCK and SCL pins
 *                  - SPI SDI and SDO pins
 *                  - Transistor switch direction
 *                  - Button direction
 *
 * parameters: none
 *
 * returns: none
 */

void GPIO_Init(void);

/*****************************************************************************/

/*
 * function name: Interrupt_Init
 *
 * description: Initialises the button and timer interrupts.
 *
 * parameters: none
 *
 * returns: none
 */

void Interrupt_Init(void);

/*****************************************************************************/

/*
 * function name: down_button_ISR
 *
 * description: Interrupt routine that decreases the value of the currently
 *              selected digit when the down button is pressed.
 *
 * parameters: none
 *
 * returns: none
 */

void down_button_ISR(void);

/*****************************************************************************/

/*
 * function name: up_button_ISR
 *
 * description: Interrupt routine that increases the value of the currently
 *              selected digit when the up button is pressed.
 *
 * parameters: none
 *
 * returns: none
 */

void up_button_ISR(void);

/*****************************************************************************/

/*
 * function name: left_button_ISR
 *
 * description: Interrupt routine that regresses the digit being selected when
 *              the left button is pressed.
 *
 * parameters: none
 *
 * returns: none
 */

void left_button_ISR(void);

/*****************************************************************************/

/*
 * function name: right_button_ISR
 *
 * description: Interrupt routine that advances the digit being selected when 
 *              the right button is pressed.
 *
 * parameters: none
 *
 * returns: none
 */

void right_button_ISR(void);

/*****************************************************************************/

/*
 * function name: mode_button_ISR
 *
 * description: Interrupt routine that advances the mode of the clock when the
 *              mode button is pressed. Begins in date selection mode, then
 *              continues to weekday selection, time selection, time display,
 *              and date display. Date display is transient; button must be
 *              held down.
 *
 * parameters: none
 *
 * returns: none
 */

void mode_button_ISR(void);

/*****************************************************************************/

/*
 * function name: Interrupt_Handler
 *
 * description: Interrupt handler for the buttons.
 *
 * parameters: none
 *
 * returns: none
 */

void interrupt Interrupt_Handler(void);
