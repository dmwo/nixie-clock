#include <xc.h>
#include <settime.h>

/*******************************************************************************
* File: settime.c
* Author: Dylan Oh
* Date Created: 23 Dec 2017
* Desc: 
*******************************************************************************/

extern volatile timedate_t param;

/******************************************************************************
 * Setting Clock Boot-up State                                                *
 ******************************************************************************/

/*
 * Initialising the nixie tube clock; setting boot-up date and time display to
 * 00/00/00, 00:00:00, mode to date, and digit to 1 (left-most tube/tens place
 * day).
 */

void initialise_nixie(timedate_t param){
    /* Setting initial date values in date array */
    param.date = {INITDAYTENS,INITDAYONES,INITMONTHTENS,INITMONTHONES,
                  INITYEARTENS,INITYEARONES};
    
    /* Setting initial time values in time array */
    param.time = {INITHOURTENS,INITHOURONES,INITMINTENS,INITMINONES,INITSECTENS,
                  INITSECONES};

    /* Setting max date values */
    param.maxdate = {2,9,5,9,5,9};
    
    /* Setting max time values */
    param.maxtime = {3,9,1,9,9,9};
    
    /* Initialising RTCC with time and date values */
    init_RTC(timedate_t param);
    
    /* Sending initial values to the nixie tubes */
    void nixie_time_out(param);
    void nixie_date_out(param);
}

/******************************************************************************
 * Button Peripheral Configuration                                            *
 ******************************************************************************/

void configure_GPIO(void){
    AD1PCFGLBITS.PCFG0
    TRISBBITS.TRISB0 = 1;   // Set pin to input
    RPINR0BITS.INT1R = ;    // Select interrupt 1 pin
    RPINR1BITS.INT2R = ;    // Select interrupt 2 pin
    
}

/*
 * Configuring up button; while the clock is in setup mode, pushing this button
 * increases the value of the nixie tube display. Otherwise, it enables setup 
 * mode.
 */

void configure_upbutton_ISR(void){   
    INTCON1BITS.NSTDIS = 1;     // Disable interrupt nesting
    IFS1BITS.INT1IF = 0;        // Clear interrupt flag on INT0
    INTCON2BITS.INT1EP = 1;     // Interrupt on negative edge
    IEC1BITS.INT1IE = 1;        // Enable interrupts on external interrupt 0
}

/*
 * Configuring mode/enter button; while the clock is in setup mode, pushing this 
 * button advances to the next digit. Otherwise, it switches between time and
 * date mode.
 */

void configure_modebutton_ISR(void){
    
}

/******************************************************************************
 * Button Interrupt Handlers                                                  *
 ******************************************************************************/

/*
 * Up button interrupt routine
 */

void interrupt upbutton(void){
    if (IFS1BITS.INT1IF & ){
        if (param.mode == DATESELECT){
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
        nixie_display(timedate_t param);
    }
}

/*
 * Enter button interrupt routine
 */

void interrupt modebutton(void){
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
 * Nixie Tube Digit Display Functions                                         *
 ******************************************************************************/

void nixie_display(timedate_t param){
    if (param.mode == NORMALOP){
        
    }
}




