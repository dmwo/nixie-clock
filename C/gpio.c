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
    TRISCBITS.TRISC3 = INPUT;  // Left button
    TRISCBITS.TRISC4 = INPUT;  // Right button
    TRISCBITS.TRISC5 = INPUT;  // Mode button
    TRISCBITS.TRISC6 = INPUT;  // Down button
    TRISCBITS.TRISC7 = INPUT;  // Up button
    RPINR0BITS.INT1R = 8;     // Map interrupt 1 to RP8
    RPINR1BITS.INT2R = 6;     // Map interrupt 2 to RP6
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
    MODE_Button_Clear();
    DOWN_Button_Clear();
    UP_Button_Clear();
    LEFT_Button_Clear();
    RIGHT_Button_Clear();

    /* Setting negative edge detection bits */
    MODE_BUTTON_NEGEDGE  = true;
    DOWN_BUTTON_NEGEDGE  = true;
    UP_BUTTON_NEGEDGE    = true;
    LEFT_BUTTON_NEGEDGE  = true;
    RIGHT_BUTTON_NEGEDGE = true;

    /* Setting positive edge detection bits */
    MODE_BUTTON_POSEDGE  = false;
    DOWN_BUTTON_POSEDGE  = false;
    UP_BUTTON_POSEDGE    = false;
    LEFT_BUTTON_POSEDGE  = false;
    RIGHT_BUTTON_POSEDGE = false;

    IFS0BITS.INT0IF = 0;    // Clear INT0 interrupt flag
    IFS1BITS.INT1IF = 0;    // Clear INT1 interrupt flag
    IFS1BITS.INT2IF = 0;    // Clear INT2 interrupt flag
    INTCON2BITS.INT0EP = 0; // Interrupt INT0 on negative edge
    INTCON2BITS.INT1EP = 0; // Interrupt INT1 on negative edge
    INTCON2BITS.INT2EP = 0; // Interrupt INT2 on negative edge
    IEC0BITS.INT0IE = 1;    // Enable INT0 interrupt
    IEC1BITS.INT1IE = 1;    // Enable INT1 interrupt
    IEC1BITS.INT2IE = 1;    // Enable INT2 interrupt
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
    // interrupt handler
    if (PIE0bits.IOCIE == 1 && PIR0bits.IOCIF == 1){
        if (IOCBFbits.IOCBF7 == 1) { mode_button_ISR(); }
        if (IOCCFbits.IOCCF3 == 1) { down_button_ISR(); }
        if (IOCCFbits.IOCCF4 == 1) { up_button_ISR(); }
        if (IOCCFbits.IOCCF6 == 1) { left_button_ISR(); }
        if (IOCCFbits.IOCCF7 == 1) { right_button_ISR(); }
    } else {}
}