#include <xc.h>
#include "pic16f18445.h"
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

/******************************************************************************
 * filename: nixie.h                                                          *
 *                                                                            *
 * purpose: Contains functions pertaining to the operation of the 
 *                                                                            *
 * date created: 1 July 2019                                                  *
 *                                                                            *
 * authors: Dylan Oh                                                          *
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS                                                          *
 *****************************************************************************/

#define SW_HRDAY  PORTAbits.RA0
#define SW_MINMON PORTBbits.RB4
#define SW_SECYR  PORTBbits.RB5
#define EN_HV     PORTCbits.RC0 //move to timer

/******************************************************************************
 * FUNCTION DECLARATIONS                                                      *
 *****************************************************************************/

/*
 * function name: convert_BCD
 *
 * description: Converts and 8-bit integer from Binary Coded Decimal to Decimal
 *              or vice-versa.
 *
 * parameters: uint8_t, value to be converted
 *             bool   , conversion type (BCD to DEC or DEC to BCD)
 *
 * returns: uint8_t, converted value
 */

uint8_t convert_BCD(uint8_t, bool);

/*****************************************************************************/

/*
 * function name: set_nixie
 *
 * description: Transfers the user-defined decimal values for time and date
 *              into the BCD structs that are used to set the shift registers.
 *              Splits numbers into tens and ones places.
 *
 * parameters: none
 *
 * returns: none
 */

void set_nixie(void);

/*****************************************************************************/

/*
 * function name: nixie_toggle
 *
 * description: Delay function that causes the currently selected digit (in
 *              clock setting mode) to flash with a 1 second period to show the
 *              user which value they are changing.
 *
 * parameters: none
 *
 * returns: none
 */

void nixie_toggle(void);