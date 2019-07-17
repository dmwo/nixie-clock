#include "gpio.h"

/******************************************************************************
 * filename: gpio.c                                                           *
 *                                                                            *
 * purpose: Contains functions pertaining to the operation of the GPIO.       *
 *          Specifically, the buttons used to set the time and date of the    *
 *          clock and the interrupts associated with their functioning.       *
 *                                                                            *
 * date created:
 *                                                                            *
 * authors: Dylan Oh                                                          *
 *****************************************************************************/

/******************************************************************************
 * Configuring Ports and Peripherals                                          *
 ******************************************************************************/

void GPIO_Init(void){
    AD1PCFGL = 0xFFFF;

    /* Setting IO direction for SPI pins */
    DIR_SDO2 = OUTPUT; // Set pin B5 as SDO2 input
    DIR_SDI2 = OUTPUT; // Set pin B4 as SDI2 input
    DIR_SCK2 = OUTPUT; // Set pin B6 as SCK2 input

    /* Setting port and pin locations for SPI */
    SSP2DATPPSbits.PIN  = SDI2_PIN;
    SSP2DATPPSbits.PORT = SDI2_PORT;
    SSP2CLKPPSbits.PIN  = SCK2_PIN;
    SSP2CLKPPSbits.PORT = SCK2_PORT;

    /* Assigning functions in peripheral pin select */
    RB5PPS = SDO2;
    RB6PPS = SCK2;

    /* Setting the IO direction for the transistor switches */
    DIR_SW_MINMON = OUTPUT; // Minute / month switch
    DIR_SW_SECYR  = OUTPUT; // Second / year switch
    DIR_SW_HRDAY  = OUTPUT; // Hour / day switch
    DIR_SW_HV     = OUTPUT; // High voltage switch

    /* Setting the IO direction for the buttons */
    DIR_BUTTON_UP    = INPUT;
    DIR_BUTTON_DOWN  = INPUT;
    DIR_BUTTON_LEFT  = INPUT;
    DIR_BUTTON_RIGHT = INPUT;
    DIR_BUTTON_MODE  = INPUT;
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

void up_button_ISR(void){
    /* Increase value of current digit unless at maximum */
    if (num[digit] == maxval[digit]) num[digit] = minval[digit];
    else num[digit]++;

    set_nixie();
    SN74HC595_Write();
    Up_Int_Clear();
    Global_Int_Enable();
}

void down_button_ISR(void){
    /* Decrease value of current digit unless at minimum */
    if (num[digit] == minval[digit]) num[digit] = maxval[digit];
    else num[digit]--;

    set_nixie();
    SN74HC595_Write();
    Down_Int_Clear();
    Global_Int_Enable();
}

void left_button_ISR(void){
    /* Cycle left through digits */
    if (mode == WKDAYSET) break;
    else if (digit == DAYSEL) digit = YRSEL;
    else if (digit == HRSEL) digit = SECSEL;
    else digit--;

    Left_Int_Clear();
    Global_Int_Enable();
}

void right_button_ISR(void){
    /* Cycle right through digits */
    if (mode == WKDAYSET) break;
    else if (digit == YRSEL) digit = DAYSEL;
    else if (digit == SECSEL) digit = HRSEL;
    else digit++;

    Right_Int_Clear();
    Global_Int_Enable();
}

void mode_button_ISR(void){
    if (mode == DATESET){
        /* Switch to weekday set mode after setting date */
        mode = WKDAYSET;
        digit = WKDSEL;
    } else if (mode == WKDAYSET){
        /* Switch to time set mode after setting weekday */
        mode = TIMESET;
        digit = HRSEL;
    } else if (mode == TIMESET){
        /* Switch to time display mode after setting time */
        mode = TIMEDISP;
        // RTCC_Set();
    } else if (mode == TIMEDISP){
        /* Switch to date display mode while button is pushed */
        MODE_BUTTON_POSEDGE = true;
        MODE_BUTTON_NEGEDGE = false;
        mode = DATEDISP;
    } else {
        /* Switch back to time display mode when button is released */
        MODE_BUTTON_POSEDGE = false;
        MODE_BUTTON_NEGEDGE = true;
        mode = TIMEDISP;
    }

    SN74HC595_Write();
    Mode_Int_Clear();
    Global_Int_Enable();
}

void interrupt Interrupt_Handler(void){
    // Global interrupt automatically disabled upon receiving interrupt

    /* Determining cause of interrupt and running appropriate ISR */
    if (PIE0bits.IOCIE == 1 && PIR0bits.IOCIF == 1){
        sleep = false;
        if (IOCBFbits.IOCBF7 == 1) mode_button_ISR();
        if (IOCCFbits.IOCCF3 == 1) down_button_ISR();
        if (IOCCFbits.IOCCF4 == 1) up_button_ISR();
        if (IOCCFbits.IOCCF6 == 1) left_button_ISR();
        if (IOCCFbits.IOCCF7 == 1) right_button_ISR();
    }
}
