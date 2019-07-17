#include "sleep.h"

/******************************************************************************
 * filename: sleep.c                                                          *
 *                                                                            *
 * purpose: 
 *                                                                            *
 * date created: 17 July 2019                                                 *
 *                                                                            *
 * authors: Dylan Oh                                                          *
 *****************************************************************************/

volatile char sleep;

/******************************************************************************
 * FUNCTION DEFINITIONS                                                       *
 *****************************************************************************/

void EMU_Sleep(void){
    VREGCON.VREGPM = 1 // enables low power sleep
    CPUDOZE.IDLEN = 0  // goes to sleep instead of idle
    if (sleep) asm SLEEP;
    else break;
}