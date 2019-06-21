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
    dateRTC.tens  = (convert_BCD(num[DAYSEL], DEC_TO_BCD) & 0xF0) >> 4;
    monthRTC.tens = (convert_BCD(num[MONSEL], DEC_TO_BCD) & 0xF0) >> 4;
    yearRTC.tens  = (convert_BCD(num[YRSEL],  DEC_TO_BCD) & 0xF0) >> 4;
    secRTC.tens   = (convert_BCD(num[SECSEL], DEC_TO_BCD) & 0xF0) >> 4;
    minRTC.tens   = (convert_BCD(num[MINSEL], DEC_TO_BCD) & 0xF0) >> 4;
    hourRTC.tens  = (convert_BCD(num[HRSEL],  DEC_TO_BCD) & 0xF0) >> 4;
    dateRTC.ones  = (convert_BCD(num[DAYSEL], DEC_TO_BCD) & 0x0F);
    monthRTC.ones = (convert_BCD(num[MONSEL], DEC_TO_BCD) & 0x0F);
    yearRTC.ones  = (convert_BCD(num[YRSEL],  DEC_TO_BCD) & 0x0F);
    secRTC.ones   = (convert_BCD(num[SECSEL], DEC_TO_BCD) & 0x0F);
    minRTC.ones   = (convert_BCD(num[MINSEL], DEC_TO_BCD) & 0x0F);
    hourRTC.ones  = (convert_BCD(num[HRSEL],  DEC_TO_BCD) & 0x0F);
    wkdayRTC.ones = num[WKDSEL];
}

void nixie_toggle(void){
    for (int i = 0; i < DELAY_TICK; i++);
    if (digit == DAYSEL || digit == HRSEL) // toggle nixies 1 and 2
    else if (digit == MONSEL || digit == MINSEL) // toggle nixies 3 and 4
    else // toggle nixies 5 and 6
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

void up_button_ISR(void){
    /* Increase value of current digit unless at maximum */
    if (num[digit] == maxval[digit]) num[digit] = minval[digit];
    else num[digit]++;

    set_nixie();
    SN74HC595_Write();
    Up_Int_Clear();
    Global_Int_Enable();
}

void left_button_ISR(void){
    /* Cycle left through digits */
    if (mode == WKDAYSET) break;
    else if (mode == DATESET && digit == YRSEL) digit = DAYSEL;
    else if (mode == TIMESET && digit == SECSEL) digit = HRSEL;
    else digit++;

    Left_Int_Clear();
    Global_Int_Enable();
}

void right_button_ISR(void){
    /* Cycle right through digits */
    if (mode == WKDAYSET) break;
    else if (mode == DATESET && digit == DAYSEL) digit = YRSEL;
    else if (mode == TIMESET && digit == HRSEL) digit = SECSEL;
    else digit--;

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
        RTCC_Set();
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
        if (IOCBFbits.IOCBF7 == 1) mode_button_ISR();
        if (IOCCFbits.IOCCF3 == 1) down_button_ISR();
        if (IOCCFbits.IOCCF4 == 1) up_button_ISR();
        if (IOCCFbits.IOCCF6 == 1) left_button_ISR();
        if (IOCCFbits.IOCCF7 == 1) right_button_ISR();
    }
}
