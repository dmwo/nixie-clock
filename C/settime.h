/* 
 * File:   settime.h
 * Author: Dylan
 *
 * Created on 23 December 2017, 19:13
 */

#ifndef SETTIME_H
#define	SETTIME_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************
 * Setting Initial Bootup Date and Time Macros for 00/00/00, 00:00:00, initial*
 * nixie tube digit, and date/time mode macros                                *
 ******************************************************************************/
    
#define INITDAYTENS       (0b0000)   // Sets initial tens place day to 0
#define INITDAYONES       (0b0000)   // Sets initial ones place day to 0
#define INITMONTHTENS     (0b0000)   // Sets initial tens place month to 0
#define INITMONTHONES     (0b0000)   // Sets initial ones place month to 0
#define INITYEARTENS      (0b0000)   // Sets initial tens place year to 0
#define INITYEARONES      (0b0000)   // Sets initial ones place year to 0
#define INITHOURTENS      (0b0000)   // Sets initial tens place hours to 0
#define INITHOURONES      (0b0000)   // Sets initial ones place hours to 0
#define INITMINTENS       (0b0000)   // Sets initial tens place minutes to 0
#define INITMINONES       (0b0000)   // Sets initial ones place minutes to 0
#define INITSECTENS       (0b0000)   // Sets initial tens place seconds to 0
#define INITSECONES       (0b0000)   // Sets initial ones place seconds to 0
#define INITDIGIT         (1)   // Sets initial nixie tube digit
#define NORMALOP		  (2)   // Sets clock to normal operation mode
#define DATESELECT        (1)   // Sets clock to date mode
#define TIMESELECT        (0)   // Sets clock to time mode
	
/*****************************************************************************
 * Creating Date and Time Structures                                         *
 *****************************************************************************/
	
typedef struct{
	volatile int date[6];
	const int maxdate[6];
	volatile int time[6];
	const int maxtime[6];
	volatile int digit;
	volatile int mode;
} timedate_t;

/*****************************************************************************
 * Function Declarations													 *
 *****************************************************************************/

void initialise_nixie(timedate_t param);

void configure_GPIO(void);

void configure_upbutton_ISR(void);
void configure_modebutton_ISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SETTIME_H */

