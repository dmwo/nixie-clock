#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * filename: rtcc.h                                                           *
 *                                                                            *
 * purpose: Contains function declarations and macros used in rtcc.c          *
 *                                                                            *
 * date created: 28 Dec 2018                                                  *
 *                                                                            *
 * authors: Dylan Oh                                                          *
 *****************************************************************************/

static const __flash uint8_t lookup[10] = 
{0, 2, 1, 3, 8, 10, 9, 11, 4, 6};

/* Function Declarations */
void init_spi(void);
void wait_spi(void);
void write_spi(uint8_t data);
void write_shift(void);
