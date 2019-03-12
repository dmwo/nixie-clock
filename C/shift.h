#include <xc.h>
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
 * transforms to CBDA, lookup_tens to BDAC.
 */

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
  @Summary
    Initialises SPI2
  
  @Description
    Routine should be called once during main and before any other MSSP2
    routines.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment


  @Example
    <code>

    </code>
 */

void SPI2_Init(void);

/*****************************************************************************/

/*
  @Summary
    Writes a byte of data over SPI2.

  @Description
    Routine writes a byte of data to a SPI peripheral through the SPI2 bus.
    Not an exchange function; RX data from the peripheral is discarded as the
    SN74HC595N does not return meaningful data.

  @Preconditions


  @Param


  @Returns


  @Comment


  @Example
    <code>

    </code>
 */

void SPI2_Write(uint8_t data);

/*****************************************************************************/

/*
  @Summary


  @Description


  @Preconditions


  @Param


  @Returns


  @Comment


  @Example
    <code>

    </code>
 */

void SPI2_Wait_Buffer(void);

/*****************************************************************************/

/*
  @Summary


  @Description


  @Preconditions


  @Param


  @Returns


  @Comment


  @Example
    <code>

    </code>
 */

void SN74HC595_Write(void);
