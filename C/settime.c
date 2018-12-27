#include <xc.h>
#include <settime.h>

/*******************************************************************************
* File: settime.c
* Author: Dylan Oh
* Date Created: 23 Dec 2017
* Desc: Contains functions that operate and retrieve data from the real
*       time clock onboard the PIC24HJ128GP504-I/PT for Nixie Tube Clock
*       application.
*******************************************************************************/

/******************************************************************************
 * Number Conversion  Functions                                               *
 ******************************************************************************/

int decimal_to_BCD(int num){
    return (((num / 10) % 10) << 4) + (num % 10);
}

int decimal_to_BCD(int BCD){
    return (((BCD & 0xF0) >> 4) * 10) + (BCD & 0x0F);
}

/******************************************************************************
 * Clock Delay Functions                                                      *
 ******************************************************************************/



void __delay_ms(){

}

/******************************************************************************
 * RTC Initialisation Functions                                               *
 ******************************************************************************/

void setRTCWREN(void){
    __asm__ ("MOV   #NVMKEY, W1");
    __asm__ ("MOV   #0x55, W2");
    __asm__ ("MOV   W2, [W1]");
    __asm__ ("MOV   #0xAA, W3");
    __asm__ ("MOV   W3, [W1]");
    __asm__ ("BSET  RCFGCAL, #13");
    return;
}

void clearRTCWREN(void){
    RCFGCALBITS.RTCWREN = 0;
    return;
}

/******************************************************************************
 * Reading and Writing to RTCC                                                *
 ******************************************************************************/

void set_RTCC(TimeDate_t* x){
    setRTCWREN();
    RCFGCALBITS.RTCPTR = 0x03;
    RCFGCALBITS.RTCEN = 0;
    RTCVAL = decimal_to_BCD(x -> year);
    RTCVAL = (decimal_to_BCD(x -> month) << 8) + decimal_to_BCD(x -> day);
    RTCVAL = decimal_to_BCD(x -> hour);
    RTCVAL = (decimal_to_BCD(x -> min) << 8) + decimal_to_BCD(x -> sec);
    RCFGCALBITS.RTCEN = 1;
    clearRTCWREN();
    return;
}

void grab_RTCC(TimeDate_t* x){
    x -> year = BCD_to_decimal(RTCVAL);
    int temp = RTCVAL;
    x -> month = BCD_to_decimal(temp >> 8);
    x -> day = BCD_to_decimal(temp & 0xFF);
    x -> hour = BCD_to_decimal(RTCVAL & 0xFF);
    temp = RTCVAL;
    x -> min = BCD_to_decimal(temp >> 8);
    x -> sec = BCD_to_decimal(temp & 0xFF);
    return;
}

/******************************************************************************
 * Configuring Ports and Peripherals                                          *
 ******************************************************************************/

void configure_GPIO(void){
    AD1PCFGL = 0xFFFF;
    /* Setting all the output pins for the nixie tubes */
    TRISBBITS.TRISB0 = OUTPUT;
    TRISBBITS.TRISB1 = OUTPUT;
    TRISBBITS.TRISB2 = OUTPUT;
    TRISBBITS.TRISB3 = OUTPUT;
    TRISBBITS.TRISB4 = OUTPUT;
    TRISBBITS.TRISB10 = OUTPUT;
    TRISBBITS.TRISB11 = OUTPUT;
    TRISBBITS.TRISB12 = OUTPUT;
    TRISBBITS.TRISB13 = OUTPUT;
    TRISBBITS.TRISB14 = OUTPUT;
    TRISBBITS.TRISB15 = OUTPUT;

    TRISCBITS.TRISC0 = OUTPUT;
    TRISCBITS.TRISC1 = OUTPUT;
    TRISCBITS.TRISC6 = OUTPUT;
    TRISCBITS.TRISC7 = OUTPUT;
    TRISCBITS.TRISC8 = OUTPUT;
    TRISCBITS.TRISC9 = OUTPUT;

    /* Setting the input pins for the buttons */
    TRISBBITS.TRISB7 = INPUT;
    TRISBBITS.TRISB8 = INPUT;
    RPINR0BITS.INT1R = 8;     // Map interrupt 1 to RP8
    RPINR1BITS.INT2R = 6;     // Map interrupt 2 to RP6
    return;
}

/******************************************************************************
 * Writing to Nixie Tubes                                                     *
 ******************************************************************************/

void nixie_display(TimeDate_t* x){
    if (x -> mode == TIMEDISPLAY || x -> mode == TIMESELECT){
        /* Setting hour tens digit */
        PORTCBITS.RC4 = decimal_to_BCD(x -> hour & 0xF0) & BCDMASKB;
        PORTCBITS.RC3 = decimal_to_BCD(x -> hour & 0xF0) & BCDMASKC;
        /* Setting hour ones digit */
        PORTBBITS.RB4 = decimal_to_BCD(x -> hour & 0x0F) & BCDMASKA;
        PORTCBITS.RC2 = decimal_to_BCD(x -> hour & 0x0F) & BCDMASKD;
        PORTCBITS.RC1 = decimal_to_BCD(x -> hour & 0x0F) & BCDMASKB;
        PORTCBITS.RC0 = decimal_to_BCD(x -> hour & 0x0F) & BCDMASKC;
        /* Setting minutes tens digit */
        PORTBBITS.RB3 = decimal_to_BCD(x -> min & 0xF0) & BCD0MASKA;
        PORTBBITS.RB2 = decimal_to_BCD(x -> min & 0xF0) & BCD1MASKB;
        PORTBBITS.RB1 = decimal_to_BCD(x -> min & 0xF0) & BCD2MASKC;
        /* Setting mintues ones digit */
        PORTBBITS.RB0  = decimal_to_BCD(x -> min & 0x0F) & BCDMASKA;
        PORTBBITS.RB15 = decimal_to_BCD(x -> min & 0x0F) & BCDMASKD;
        PORTBBITS.RB14 = decimal_to_BCD(x -> min & 0x0F) & BCDMASKB;
        PORTBBITS.RB13 = decimal_to_BCD(x -> min & 0x0F) & BCDMASKC;
        /* Setting seconds tens digit */
        PORTBBITS.RB12 = 0;
        PORTBBITS.RB11 = decimal_to_BCD(x -> sec & 0xF0) & BCDMASKA;
        PORTBBITS.RB10 = decimal_to_BCD(x -> sec & 0xF0) & BCDMASKB;
        PORTBBITS.RB9  = decimal_to_BCD(x -> sec & 0xF0) & BCDMASKC;
        /* Setting seconds ones digit */
        PORTCBITS.RC8 = decimal_to_BCD(x -> sec & 0x0F) & BCDMASKA;
        PORTCBITS.RC7 = decimal_to_BCD(x -> sec & 0x0F) & BCDMASKD;
        PORTCBITS.RC6 = decimal_to_BCD(x -> sec & 0x0F) & BCDMASKB;
        PORTBBITS.RB9 = decimal_to_BCD(x -> sec & 0x0F) & BCDMASKC;
        
        if (x -> mode == TIMESELECT){
            switch(x -> digit){
                case HRDAY:
                /* Making hour/year digits flash while others are steady */

                case MINMON:

                case SECYR:
            }
        }
    } else if (x -> mode == DATEDISPLAY){
        /* Setting day tens digit */
        PORTABITS.RA4 = decimal_to_BCD(x -> day & 0xF0) & BCDMASKB;
        PORTABITS.RA3 = decimal_to_BCD(x -> day & 0xF0) & BCDMASKC;
        /* Setting day ones digit */
        PORTBBITS.RB4 = decimal_to_BCD(x -> day & 0x0F) & BCDMASKA;
        PORTCBITS.RC2 = decimal_to_BCD(x -> day & 0x0F) & BCDMASKD;
        PORTCBITS.RC1 = decimal_to_BCD(x -> day & 0x0F) & BCDMASKB;
        PORTCBITS.RC0 = decimal_to_BCD(x -> day & 0x0F) & BCDMASKC;
        /* Setting month tens digit */
        PORTBBITS.RB3 = 0;
        PORTBBITS.RB2 = 0;
        PORTBBITS.RB1 = decimal_to_BCD(x -> month & 0xF0) & BCDMASKA;
        /* Setting month ones digit */
        PORTBBITS.RB0  = decimal_to_BCD(x -> month & 0x0F) & BCD3MASK;
        PORTBBITS.RB15 = decimal_to_BCD(x -> month & 0x0F) & BCD2MASK;
        PORTBBITS.RB14 = decimal_to_BCD(x -> month & 0x0F) & BCD1MASK;
        PORTBBITS.RB13 = decimal_to_BCD(x -> month & 0x0F) & BCD0MASK;
        /* Setting year tens digit */
        PORTBBITS.RB12 = decimal_to_BCD(x -> year & 0xF0) & BCD3MASK;
        PORTBBITS.RB11 = decimal_to_BCD(x -> year & 0xF0) & BCD2MASK;
        PORTBBITS.RB10 = decimal_to_BCD(x -> year & 0xF0) & BCD1MASK;
        PORTBBITS.RB9  = decimal_to_BCD(x -> year & 0xF0) & BCD0MASK;
        /* Setting year ones digit */
        PORTCBITS.RC8 = decimal_to_BCD(x -> year & 0x0F) & BCD3MASK;
        PORTCBITS.RC7 = decimal_to_BCD(x -> year & 0x0F) & BCD2MASK;
        PORTCBITS.RC6 = decimal_to_BCD(x -> year & 0x0F) & BCD1MASK;
        PORTBBITS.RB9 = decimal_to_BCD(x -> year & 0x0F) & BCD0MASK;
    } else if (x -> mode == DATESELECT){

    }
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

/******************************************************************************
 * Setting Clock Boot-up State                                                *
 ******************************************************************************/

/*
 * Initialising the nixie tube clock; setting boot-up date and time display to
 * 00/00/00, 00:00:00, mode to date, and digit to 1 (left-most tube/tens place
 * day).
 */


