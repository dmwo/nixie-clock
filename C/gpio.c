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

uint8_t convert_BCD(uint8_t val, bool dir){
    if (dir == BCD_TO_DEC) return ((val >> 4) * 10 + (val & 0xF));
    else return (((val / 10) << 4) | (val % 10));
}

void set_nixie(void){
    if (mode == DATESEL){
        dateRTC.tens  = (convert_BCD(num[DAYSEL], DEC_TO_BCD) & 0xF0) >> 4;
        dateRTC.ones  =  convert_BCD(num[DAYSEL], DEC_TO_BCD) & 0x0F;
        monthRTC.tens = (convert_BCD(num[MONSEL], DEC_TO_BCD) & 0xF0) >> 4;
        monthRTC.ones =  convert_BCD(num[MONSEL], DEC_TO_BCD) & 0x0F;
        yearRTC.tens  = (convert_BCD(num[YRSEL], DEC_TO_BCD)  & 0xF0) >> 4;
        yearRTC.ones  =  convert_BCD(num[YRSEL], DEC_TO_BCD)  & 0x0F;
    } else if (mode == WKDSEL){
        wkdayRTC.ones = num[WKDSEL];
    } else {
        secRTC.tens  = (convert_BCD(num[SECSEL], DEC_TO_BCD) & 0xF0) >> 4;
        secRTC.ones  =  convert_BCD(num[SECSEL], DEC_TO_BCD) & 0x0F;
        minRTC.tens  = (convert_BCD(num[MINSEL], DEC_TO_BCD) & 0xF0) >> 4;
        minRTC.ones  =  convert_BCD(num[MINSEL], DEC_TO_BCD) & 0x0F;
        hourRTC.tens = (convert_BCD(num[HRSEL],  DEC_TO_BCD) & 0xF0) >> 4;
        hourRTC.ones =  convert_BCD(num[HRSEL],  DEC_TO_BCD) & 0x0F;
        RTCC_Set();
    }
}

void nixie_toggle(void){

}

void down_button_ISR(void){
    if (mode == DATESEL){
        if (digit == YRSEL && num[digit] == YRMIN) num[digit] = YRMAX;
        else if (digit == MONSEL && num[digit] == MONMIN) num[digit] = MONMAX;
        else if (digit == DAYSEL && num[digit] == DAYMIN) num[digit] = DAYMAX;
    } else if (mode == WKDAYSEL){
        if (digit == WKDSEL && num[digit] == WKDMIN) num[digit] = WKDMAX;
    } else if (mode == TIMESEL){
        if (digit == SECSEL && num[digit] == SECMIN) num[digit] = SECMAX;
        else if (digit == MINSEL && num[digit] == MINMIN) num[digit] = MINMAX;
        else if (digit == HRSEL && num[digit] == HRMIN) num[digit] = HRMAX;
    } else num[digit]--;

    set_nixie();
    SN74HC595_Write();
    Down_Int_Clear();
    Global_Int_Enable();
}

void up_button_ISR(void){
    if (mode == DATESEL){
        if (digit == YRSEL && num[digit] == YRMAX) num[digit] = 0;
        else if (digit == MONSEL && num[digit] == MONMAX) num[digit] = MONMIN;
        else if (digit == DAYSEL && num[digit] == DAYMAX) num[digit] = DAYMIN;
    } else if (mode == WKDAYSEL){
        if (digit == WKDSEL && num[digit] == WKDMAX) num[digit] = WKDMIN;
    } else if (mode == TIMESEL){
        if (digit == SECSEL && num[digit] == SECMAX) num[digit] = 0;
        else if (digit == MINSEL && num[digit] == MINMAX) num[digit] = 0;
        else if (digit == HRSEL && num[digit] == HRMAX) num[digit] = 0;
    } else num[digit]++;

    set_nixie();
    SN74HC595_Write();
    Up_Int_Clear();
    Global_Int_Enable();
}

void left_button_ISR(void){
    /* Cycle left through digits */
    if (mode == WKDAYSEL) return;
    else if (digit == 2) digit == 0;
    else digit++;

    Left_Int_Clear();
    Global_Int_Enable();
}

void right_button_ISR(void){
    /* Cycle right through digits */
    if (mode == WKDAYSEL) return;
    else if (digit == 0) digit = 2;
    else digit--;

    Right_Int_Clear();
    Global_Int_Enable();
}

void mode_button_ISR(void){
    if (mode == DATESEL){
        mode = WKDAYSEL;
        digit = 0;
        num[0] = 0;
    } else if (mode == WKDAYSEL){
        /* Switch to time set mode after setting date */
        mode = TIMESEL;
        digit = 0;
        for (int i = 0; i < 3; i++) num[i] = 0;
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
