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

int decimal_to_BCD(int num){
    return (((num / 10) % 10) << 4) + (num % 10);
}

int decimal_to_BCD(int BCD){
    return (((BCD & 0xF0) >> 4) * 10) + (BCD & 0x0F);
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
}

void clearRTCWREN(void){
    RCFGCALBITS.RTCWREN = 0;
}

/******************************************************************************
 * Reading and Writing to RTCC                                                *
 ******************************************************************************/

void set_RTCC(TimeDate_t* param){
    setRTCWREN();
    RCFGCALBITS.RTCPTR = 0x03;
    RCFGCALBITS.RTCEN = 0;
    RTCVAL = decimal_to_BCD(param -> year);
    RTCVAL = (decimal_to_BCD(param -> month) << 8) + decimal_to_BCD(param -> day);
    RTCVAL = decimal_to_BCD(param -> hour);
    RTCVAL = (decimal_to_BCD(param -> min) << 8) + decimal_to_BCD(param -> sec);
    RCFGCALBITS.RTCEN = 1;
    clearRTCWREN();
}

void grab_RTCC(TimeDate_t* param){
    param -> year = BCD_to_decimal(RTCVAL);
    int temp = RTCVAL;
    param -> month = BCD_to_decimal(temp >> 8);
    param -> day = BCD_to_decimal(temp & 0xFF);
    param -> hour = BCD_to_decimal(RTCVAL & 0xFF);
    temp = RTCVAL;
    param -> min = BCD_to_decimal(temp >> 8);
    param -> sec = BCD_to_decimal(temp & 0xFF);
}

/******************************************************************************
 * Configuring Ports and Peripherals                                          *
 ******************************************************************************/

void configure_GPIO(void){
    /* Setting all the output pins for the nixie tubes */
    TRISABITS.TRISA0 = OUTPUT;
    TRISABITS.TRISA1 = OUTPUT;
    TRISABITS.TRISA2 = OUTPUT;
    TRISABITS.TRISA3 = OUTPUT;
    TRISABITS.TRISA7 = OUTPUT;
    TRISABITS.TRISA8 = OUTPUT;
    TRISABITS.TRISA10 = OUTPUT;

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

    /*  */

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
 * Reading and Writing to Nixie Tubes                                         *
 ******************************************************************************/

void nixie_display(TimeDate_t* param){
    if (param -> mode == TIMEDISPLAY || param -> mode == TIMESELECT){
        
    } else if (param -> mode == DATEDISPLAY || param -> mode == DATESELECT){

    } else {

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

void initialise_nixie(TimeDate_t* param){
    /* Setting initial date values in date array */
    param -> date = {INITDAYTENS,INITDAYONES,INITMONTHTENS,INITMONTHONES,
                  INITYEARTENS,INITYEARONES};
    
    /* Setting initial time values in time array */
    param -> time = {INITHOURTENS,INITHOURONES,INITMINTENS,INITMINONES,INITSECTENS,
                  INITSECONES};

    /* Setting max date values */
    param -> maxdate = {2,9,5,9,5,9};
    
    /* Setting max time values */
    param -> maxtime = {3,9,1,9,9,9};
    
    /* Initialising RTCC with time and date values */
    init_RTC();
    
    /* Sending initial values to the nixie tubes */
    void nixie_time_out(&param);
    void nixie_date_out(&param);
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
        nixie_display(Nixie_t param);
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

void nixie_display(TimeDate_t param){
    if (param.mode == NORMALOP){
        
    }
}




