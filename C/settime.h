/* 
 * File:   settime.h
 * Author: Dylan
 *
 * Created on 23 December 2017, 19:13
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif

#ifndef SETTIME_H
#define	SETTIME_H

/******************************************************************************
 * Setting Initial Bootup Date and Time Macros for 00/00/00, 00:00:00, initial*
 * nixie tube digit, and date/time mode macros                                *
 ******************************************************************************/

#define OUTPUT            (0)
#define INPUT             (1)
#define INITDIGIT         (1)   	 // Sets initial nixie tube digit
#define DATEDISPLAY       (3)        // Sets clock to date display mode
#define TIMEDISPLAY		  (2)  		 // Sets clock to time display mode
#define DATESELECT        (1)   	 // Sets clock to date mode
#define TIMESELECT        (0)   	 // Sets clock to time mode
	
/*****************************************************************************
 * Creating Date and Time Structures                                         *
 *****************************************************************************/
	
typedef struct{
	volatile int year = 0, month = 0, day = 0;
	volatile int hour = 0, min = 0, sec = 0;
	volatile int mode;
} TimeDate_t;

/*****************************************************************************
 * Function Declarations													 *
 *****************************************************************************/

void initialise_nixie(TimeDate_t param);

void configure_GPIO(void);

void configure_upbutton_ISR(void);
void configure_modebutton_ISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SETTIME_H */

