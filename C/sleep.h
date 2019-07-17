#include <xc.h>
#include "pic16f18445"

/******************************************************************************
 * filename: sleep.h                                                          *
 *                                                                            *
 * purpose: Contains functions pertaining to the management of sleep modes.   *
 *                                                                            *
 * date created: 17 July 2019                                                 *
 *                                                                            *
 * authors: Dylan Oh                                                          *
 *****************************************************************************/

extern volatile char sleep;

/******************************************************************************
 * MACRO DEFINITIONS                                                          *
 *****************************************************************************/

void EMU_Sleep(void);