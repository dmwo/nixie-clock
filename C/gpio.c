#include "gpio.h"

/******************************************************************************
 * Configuring Ports and Peripherals                                          *
 ******************************************************************************/

void GPIO_Init(void){
    AD1PCFGL = 0xFFFF;

    /* Setting SPI pins */
    
    TRISBbits.TRISB5 = OUTPUT; // Set pin B5 as SDO2 input
    RB5PPS = SDO2;

    TRISBbits.TRISB4    = OUTPUT; // Set pin B4 as SDI2 input
    SSP2DATPPSbits.PIN  = SDI2_PIN;
    SSP2DATPPSbits.PORT = SDI2_PORT;

    TRISBbits.TRISB6    = OUTPUT; // Set pin B6 as SCK2 input
    SSP2CLKPPSbits.PIN  = SCK2_PIN;
    SSP2CLKPPSbits.PORT = SCK2_PORT;
    RB6PPS = SCK2;

    /* Setting all the output pins for the transistor switches */
    TRISABITS.TRISA0 = OUTPUT; // Minute / month switch
    TRISABITS.TRISA1 = OUTPUT; // Second / year switch
    TRISCBITS.TRISC0 = OUTPUT; // Hour / day switch
    TRISCBITS.TRISC1 = OUTPUT; // High voltage switch

    /* Setting the IO direction for the buttons */
    UP_BUTTON    = INPUT; // Up button
    DOWN_BUTTON  = INPUT; // Down button
    LEFT_BUTTON  = INPUT; // Left button
    RIGHT_BUTTON = INPUT; // Right button
    MODE_BUTTON  = INPUT; // Mode button
}

/******************************************************************************
 * External Interrupt Initialisation                                          *
 ******************************************************************************/

void Interrupt_Init(void){
    Nested_Int_Disable(); // Disable interrupt nesting

    /* Clearing flags for IOC interrups */
    Up_Int_Clear();
    Down_Int_Clear();
    Left_Int_Clear();
    Right_Int_Clear();
    Mode_Int_Clear();

    /* Setting negative edge detection bits */
    UP_BUTTON_NEGEDGE    = true;
    DOWN_BUTTON_NEGEDGE  = true;
    LEFT_BUTTON_NEGEDGE  = true;
    RIGHT_BUTTON_NEGEDGE = true;
    MODE_BUTTON_NEGEDGE  = true;

    /* Setting positive edge detection bits */
    UP_BUTTON_POSEDGE    = false;
    DOWN_BUTTON_POSEDGE  = false;
    LEFT_BUTTON_POSEDGE  = false;
    RIGHT_BUTTON_POSEDGE = false;
    MODE_BUTTON_POSEDGE  = false;

    IOC_Int_Enable();
    Timer_Int_Enable();
}

void down_button_ISR(void) {

    // Add custom IOCCF3 code

    SN74HC595_Write();
    Down_Int_Clear();
    Global_Int_Enable();
}

void up_button_ISR(void) {

    // Add custom IOCCF4 code

    SN74HC595_Write();
    Up_Int_Clear();
    Global_Int_Enable();
}

void left_button_ISR(void) {
    if (param.digit == 1){
        param.digit == 3;
    } else {
        param.digit--;
    }

    SN74HC595_Write();
    Left_Int_Clear();
    Global_Int_Enable();
}

void right_button_ISR(void) {

    if (param.digit == 3){
        param.digit = 1;
    } else {
        param.digit++;
    }

    SN74HC595_Write();
    Right_Int_Clear();
    Global_Int_Enable();
}

void mode_button_ISR(void) {
    if (mode == DATESEL){
        /* Switch to time set mode after setting date */
        mode = TIMESEL;
        digit = 1;
    } else if (mode == TIMESEL){
        /* Switch to time display mode after setting time */
        mode = TIME;
    } else if {mode == TIME){
        /* Switch to date display mode while button is pushed */
        MODE_BUTTON_POSEDGE = true;
        MODE_BUTTON_NEGEDGE = false;
        mode = DATE;
    } else {
        /* Switch back to time display mode when button is released */
        MODE_BUTTON_POSEDGE = false;
        MODE_BUTTON_NEGEDGE = true;
        mode = TIME;
    }

    SN74HC595_Write();
    Mode_Int_Clear();
    Global_Int_Enable();
}

void interrupt Interrupt_Handler(void){
    // Global interrupt automatically disabled upon receiving interrupt

    /* Determining cause of interrupt and running appropriate ISR */
    if (PIE0bits.IOCIE == 1 && PIR0bits.IOCIF == 1){
        if (IOCBFbits.IOCBF7 == 1) mode_button_ISR();
        if (IOCCFbits.IOCCF3 == 1) down_button_ISR();
        if (IOCCFbits.IOCCF4 == 1) up_button_ISR();
        if (IOCCFbits.IOCCF6 == 1) left_button_ISR();
        if (IOCCFbits.IOCCF7 == 1) right_button_ISR();
    }
}
