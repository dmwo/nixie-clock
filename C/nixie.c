#include <nixie.h>

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
    IOCBFbits.IOCBF7 = 0;
    IOCCFbits.IOCCF3 = 0;
    IOCCFbits.IOCCF4 = 0;
    IOCCFbits.IOCCF6 = 0;
    IOCCFbits.IOCCF7 = 0;

    /* Setting negative edge detection bits */
    IOCBNbits.IOCBN7 = 1;
    IOCCNbits.IOCCN3 = 0;
    IOCCNbits.IOCCN4 = 0;
    IOCCNbits.IOCCN6 = 0;
    IOCCNbits.IOCCN7 = 0;

    /* Setting positive edge detection bits */
    IOCBPbits.IOCBP7 = 1;
    IOCCPbits.IOCCP3 = 1;
    IOCCPbits.IOCCP4 = 1;
    IOCCPbits.IOCCP6 = 1;
    IOCCPbits.IOCCP7 = 1;

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
 * Configuring up button; while the clock is in setup mode, pushing this button
 * increases the value of the nixie tube display. Otherwise, it enables setup 
 * mode.
 */

void __attribute__((__interrupt__, auto_psv )) _ISR _INT2Interrupt (void){
    IFS1BITS.INT2IF = 0;
    IEC1BITS.INT2IE = 0;
    /* Configuring hours digits first */
    switch(param.mode){
        case TIMESELECT:
            if (param.digit == HRDAY){
                if (param.hour == 24){ param.hour = 0; }
                else { param.hour++; }
                nixie_display(&param);
                __delay_ms(1000);
                nixie_display
            } else if (param -> digit == MINMON){

            } else if (param -> digit == SECYR){

            } else {

            }
            __delay_ms(FIXTHIS);
        case DATESELECT:

        case TIMEDISPLAY:

        case DATEDISPLAY:
    }
    if (param.mode == TIMESELECT){
        if (param.date[param.digit] == param.maxdate[param.digit]){
            param.date[param.digit] = 0;
        }
        else{
            param.date[param.digit] += 1;
        }
    }
    if (param.mode == TIMESELECT){
        if (param.time[param.digit] == param.maxtime[param.digit]){
            param.time[param.digit] = 0;
        }
        else{
            param.time[param.digit] += 1;
        }
    }
    if (param.mode == NORMALOP){
        param.mode = DATESELECT;
        param.digit = 1;
    }
    nixie_display(Nixie_t param);
}

void __attribute__((__interrupt__, auto_psv )) _ISR _INT1Interrupt (void){
    IFS1BITS.INT1IF = 0;
    IEC1BITS.INT1IE = 0;
}

/*
 * Mode Button Interrupt Service Routine
 */

void IOCBF7_ISR(void) {
    // IFS0BITS.INT0IF = 0;
    // IEC0BITS.INT0IE = 0;
    if (param.mode == DATESELECT && param.digit == 6){
        param.mode = TIMESELECT;
        param.digit = 1;
    } else if (param.mode == DATESELECT && param.digit != 6){
        param.digit += 1;
    } else if (param.mode == TIMESELECT && param.digit == 6){
        param.mode = NORMALOP;
        param.digit = 1;
    } else if (param.mode == TIMESELECT && param.digit != 6){
        param.digit += 1;
    } else {
        
    }

    IOCBFbits.IOCBF7 = 0;
}

/*
 * Down Button Interrupt Service Routine
 */

void IOCCF3_ISR(void) {

    // Add custom IOCCF3 code

    IOCCFbits.IOCCF3 = 0;
}

/*
 * Up Button Interrupt Service Routine
*/
void IOCCF4_ISR(void) {

    // Add custom IOCCF4 code

    IOCCFbits.IOCCF4 = 0;
}

/*
 * Left Button Interrupt Service Routine
 */
void IOCCF6_ISR(void) {

    if (param.digit == 1){
        param.digit == 6;
    } else {
        param.digit--;
    }

    IOCCFbits.IOCCF6 = 0;
}

/*
 * Right Button Interrupt Service Routine
 */
void IOCCF7_ISR(void) {

    if (param.digit == 6){
        param.digit = 1;
    } else {
        param.digit++;
    }

    IOCCFbits.IOCCF7 = 0;
}

void __interrupt() INTERRUPT_InterruptManager (void){
    // interrupt handler
    if (PIE0bits.IOCIE == 1 && PIR0bits.IOCIF == 1){
        if (IOCBFbits.IOCBF7 == 1){
            IOCBF7_ISR();  
        }
        if (IOCCFbits.IOCCF3 == 1){
            IOCCF3_ISR();  
        }
        if (IOCCFbits.IOCCF4 == 1){
            IOCCF4_ISR();  
        }
        if (IOCCFbits.IOCCF6 == 1){
            IOCCF6_ISR();  
        }
        if (IOCCFbits.IOCCF7 == 1){
            IOCCF7_ISR();  
        }
    } else if (INTCONbits.PEIE == 1){
        if (PIE3bits.BCL2IE == 1 && PIR3bits.BCL2IF == 1){
            I2C2_BusCollisionISR();
        } else if (PIE3bits.SSP2IE == 1 && PIR3bits.SSP2IF == 1){
            I2C2_ISR();
        } else {
            //Unhandled Interrupt
        }
    } else {
        //Unhandled Interrupt
    }
}