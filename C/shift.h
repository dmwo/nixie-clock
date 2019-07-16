#include <xc.h>
#include "pic16f18445.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * filename: shift.h                                                          *
 *                                                                            *
 * purpose: Contains function declarations and macros used in shift.c.        *
 *                                                                            *
 * date created: 28 Dec 2018                                                  *
 *                                                                            *
 * author: Dylan Oh                                                           *
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS                                                          *
 *****************************************************************************/

/* 
 * Lookup tables change the order of the BCD inputs so that they are fed into
 * the shift registers properly. Original BCD in the form DBCA, lookup_ones
 * transforms to ADBC, lookup_tens to CADB.
 */
//CHANGE
static const __flash uint8_t lookup_ones[10] = 
{0, 1, 4, 5, 8, 9, 12, 13, 2, 3};

static const __flash uint8_t lookup_tens[10] =
{0, 2, 8, 10, 1, 3, 9, 11, 4, 6};

#define SPI_Status_Buffer   (SSP2STATbits.BF)

// SPI operation aliases
#define MSSP2_SPI_Enable()  (SSP2CON1bits.SSPEN = 1)
#define SPI2_Master_Mode()  (SSP2CON1bits.SSPM = 0)
#define SPI2_Clear_Status() (SSP2STAT = 0)
#define SPI2_Write_Clear()  (SSP2CON1bits.WCOL = 0)

/******************************************************************************
 * FUNCTION DECLARATIONS                                                      *
 *****************************************************************************/

/*
 * function name: SPI2_Init
 *
 * description: Initialises the MSSP2 to run in SPI mode.
 *
 * parameters: none
 *
 * returns: none
 */

void SPI2_Init(void);

/*****************************************************************************/

/*
 * function name: SPI2_Write
 *
 * description: Routine writes a byte of data to a SPI peripheral through the 
 *              SPI2 bus. Not an exchange function; RX data from the peripheral
 *              is discarded as the SN74HC595N does not return meaningful data.
 *
 * parameters: uint8_t, data to be written to the SN74HC595N shift registers
 *
 * returns: none
 */

void SPI2_Write(uint8_t);

/*****************************************************************************/

/*
 * function name: SPI2_Wait_Buffer
 *
 * description: Waits for the SPI buffer to empty (implying that a write has
 *              completed).
 *
 * parameters: none
 *
 * returns: none
 */

void SPI2_Wait_Buffer(void);

/*****************************************************************************/

/*
 * function name: SN74HC595_Write
 *
 * description: Writes data to the registers:
 *                  - time if in time set or time display modes
 *                  - date if in date set or date display modes
 *                  - weekday if in weekday set mode
 *
 * parameters: none
 *
 * returns: none
 */

void SN74HC595_Write(void);
