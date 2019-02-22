#include "gpio.h"

/******************************************************************************
 * Configuring Ports and Peripherals                                          *
 ******************************************************************************/

void init_GPIO(void){
    AD1PCFGL = 0xFFFF;
    /* Setting all the output pins for the transistor switches */
    TRISABITS.TRISA0 = OUTPUT; // Minute / month switch
    TRISABITS.TRISA1 = OUTPUT; // Second / year switch
    TRISCBITS.TRISC0 = OUTPUT; // Hour / day switch
    TRISCBITS.TRISC1 = OUTPUT; // High voltage switch

    /* Setting the input pins for the buttons */
    UP_BUTTON_PIN    = INPUT; // Up button
    DOWN_BUTTON_PIN  = INPUT; // Down button
    LEFT_BUTTON_PIN  = INPUT; // Left button
    RIGHT_BUTTON_PIN = INPUT; // Right button
    MODE_BUTTON_PIN  = INPUT; // Mode button
}

/******************************************************************************
 * External Interrupt Initialisation                                          *
 ******************************************************************************/

void enable_interrupts(void){
    /* Sets CPU interrupt priority level to 0 (highest) and enables level 1-7
     * interrupts */
    SRBITS.IPL = 0;
}

void disable_interrupts(void){
    SRBITS.IPL = 7;
}

void init_interrupts(void){
    INTCON1BITS.NSTDIS = 1; // Disable interrupt nesting

    /* Clearing flags for IOC interrups */
    UP_Button_Clear();
    DOWN_Button_Clear();
    LEFT_Button_Clear();
    RIGHT_Button_Clear();
    MODE_Button_Clear();

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

    PIE0bits.IOCIE = 1;
}

/*
 * Mode Button Interrupt Service Routine
 */

void mode_button_ISR(void) {
    if (param.mode == DATESELECT){
        param.mode = TIMESELECT;
        param.digit = 1;
    } else if (param.mode == TIMESELECT){
        param.mode = TIME;
    } else if {param.mode == TIME){
        MODE_BUTTON_POSEDGE = true;
        MODE_BUTTON_NEGEDGE = false;
        param.mode = DATE;
    } else {
        MODE_BUTTON_POSEDGE = false;
        MODE_BUTTON_NEGEDGE = true;
        param.mode = TIME;
    }

    write_shift();
    MODE_Button_Clear();
}

/*
 * Down Button Interrupt Service Routine
 */

void down_button_ISR(void) {

    // Add custom IOCCF3 code

    DOWN_Button_Clear();
}

/*
 * Up Button Interrupt Service Routine
*/
void up_button_ISR(void) {

    // Add custom IOCCF4 code

    UP_Button_Clear();
}

/*
 * Left Button Interrupt Service Routine
 */
void left_button_ISR(void) {

    if (param.digit == 1){
        param.digit == 6;
    } else {
        param.digit--;
    }

    LEFT_Button_Clear();
}

/*
 * Right Button Interrupt Service Routine
 */
void right_button_ISR(void) {

    if (param.digit == 6){
        param.digit = 1;
    } else {
        param.digit++;
    }

    RIGHT_Button_Clear();
}

void interrupt Interrupt_Handler(void){
    if (PIE0bits.IOCIE == 1 && PIR0bits.IOCIF == 1){
        if (IOCBFbits.IOCBF7 == 1) mode_button_ISR();
        if (IOCCFbits.IOCCF3 == 1) down_button_ISR();
        if (IOCCFbits.IOCCF4 == 1) up_button_ISR();
        if (IOCCFbits.IOCCF6 == 1) left_button_ISR();
        if (IOCCFbits.IOCCF7 == 1) right_button_ISR();
    }
}
