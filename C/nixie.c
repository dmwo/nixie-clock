#include <nixie.h>

/******************************************************************************
 * Configuring Ports and Peripherals                                          *
 ******************************************************************************/

void configure_GPIO(void){
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
    return;
}

/******************************************************************************
 * External Interrupt Initialisation                                          *
 ******************************************************************************/

void enable_interrupts(void){
    /* Sets CPU interrupt priority level to 0 (highest) and enables level 1-7
     * interrupts */
    SRBITS.IPL = 0;
    return;
}

void disable_interrupts(void){
    SRBITS.IPL = 7;
    return;
}

void init_interrupts(void){
    INTCON1BITS.NSTDIS = 1; // Disable interrupt nesting
    IFS0BITS.INT0IF = 0;    // Clear INT0 interrupt flag
    IFS1BITS.INT1IF = 0;    // Clear INT1 interrupt flag
    IFS1BITS.INT2IF = 0;    // Clear INT2 interrupt flag
    INTCON2BITS.INT0EP = 0; // Interrupt INT0 on negative edge
    INTCON2BITS.INT1EP = 0; // Interrupt INT1 on negative edge
    INTCON2BITS.INT2EP = 0; // Interrupt INT2 on negative edge
    IEC0BITS.INT0IE = 1;    // Enable INT0 interrupt
    IEC1BITS.INT1IE = 1;    // Enable INT1 interrupt
    IEC1BITS.INT2IE = 1;    // Enable INT2 interrupt
    return;
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
 * Configuring mode/enter button; while the clock is in setup mode, pushing this 
 * button advances to the next digit. Otherwise, it switches between time and
 * date mode.
 */

void __attribute__((__interrupt__, auto_psv )) _ISR _INT0Interrupt (void){
    IFS0BITS.INT0IF = 0;
    IEC0BITS.INT0IE = 0;
    if (param.mode == DATESELECT && param.digit == 6){
        param.mode = TIMESELECT;
        param.digit = 1;
    }
    else if (param.mode == DATESELECT && param.digit != 6){
        param.digit += 1;
    }
    else if (param.mode == TIMESELECT && param.digit == 6){
        param.mode = NORMALOP;
        param.digit = 1;
    }
    else if (param.mode == TIMESELECT && param.digit != 6){
        param.digit += 1;
    }
    else{
        
    }
}