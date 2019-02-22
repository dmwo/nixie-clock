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
 * author: Dylan Oh                                                          *
 *****************************************************************************/

static const __flash uint8_t lookup_ones[10] = 
{0, 1, 4, 5, 8, 9, 12, 13, 2, 3};

static const __flash uint8_t lookup_tens[10] =
{0, 2, 8, 10, 1, 3, 9, 11, 4, 6};

#define SPI_Wait_Buffer()          (while(!SSPSTATbits.BF))
#define SPI_Write_Clear()          (SSP2CON1bits.WCOL = 0)

/* Function Declarations */
void init_spi(void);
void write_spi(uint8_t data);
void write_shift(void);
