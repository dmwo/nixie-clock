#include <xc.h>
#include "pic16f18445.h"
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

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif

#ifndef RTCC_H
#define	RTCC_H

/******************************************************************************
 * MACRO DEFINITIONS                                                          *
 *****************************************************************************/
#define _XTAL_FREQ (16000000)
#define CTL_IN_R   (0b11011111)
#define CTL_IN_W   (0b11011110)
#define INPUT      (1)
#define OUTPUT     (0)
#define SEC_ADDR   (0x00)

#define I2C_Status_ACK      (SSP1CON2bits.ACKSTAT)
#define I2C_Status_Start    (SSP2CON2bits.SEN)
#define I2C_Status_Restart  (SSP2CON2bits.RSEN)
#define I2C_Status_Stop     (SSP2CON2bits.PEN)

#define MSSP1_I2C_Enable()  (SSP2CON1bits.SSPEN = 1)
#define I2C1_Master_Mode()  (SSP2CON1bits.SSPM = 0x8)
#define I2C1_Clear_Status() (SSPSTAT = 0)
#define I2C_Send_Start()    (SSP2CON2bits.SEN = 1)
#define I2C_Send_Restart()  (SSP2CON2bits.RSEN = 1)
#define I2C_Send_Stop()     (SSP2CON2bits.PEN = 1)

/* 
 * RTCC_t is a struct that contains information for each element of the RTCC.
 * Each 8-bit register stores the ones place of the element as a BCD value in
 * the lower halfword and the tens place of the element in the higher halfword
 *
 * In some RTCC registers, an extra bit is included that is not related to the
 * general functioning of the register but still has important information. The
 * extra element of the struct accounts for these bits, which are:
 * 
 * name     desc                          bit   register
 * ----     ----                          ---   --------
 * ST       starts oscillator             7     RTCSEC
 * 12/~24   specifies time format         6     RTCHOUR
 * OSCRUN   defines oscillator status     5     RTCWKDAY
 * LPYR     determines if leap year       5     RTCMTH
 */

typedef struct {
    uint8_t ones;
    uint8_t tens;
    uint8_t extra;
} RTCC_t;

extern volatile RTCC_t secRTC;
extern volatile RTCC_t minRTC;
extern volatile RTCC_t hourRTC;
extern volatile RTCC_t wkdayRTC;
extern volatile RTCC_t dateRTC;
extern volatile RTCC_t monthRTC;
extern volatile RTCC_t yearRTC;

/******************************************************************************
 * FUNCTION DECLARATIONS                                                      *
 *****************************************************************************/

/*
 * function name: I2C1_Init
 *
 * description: Initialises the I2C by:
 *              - setting the SCL and SDA ports in peripheral pin select
 *              - enabling MSSP1
 *              - setting I2C mode to master
 *              - setting the baud rate
 *
 * arguments: uint8_t, desired baud rate of the I2C
 * 
 * returns: none
 */

void I2C1_Init(int);

/*****************************************************************************/

/*
 * function name: I2C1_Wait
 *
 * description: Waits for an ACK from a slave device by polling if data is
 *              being transmitted over the I2C bus or if there is either a
 *              NACK or no ACK
 *
 * arguments: none
 *
 * returns: none
 */

void I2C1_Wait(void);

/*****************************************************************************/

/*
 * function name: I2C1_Start
 *
 * description: Sends a start condition to the I2C. The start enable bit is
 *              automatically cleared by the PIC16F18445, so when that bit is
 *              deasserted, the start condition is successful
 *
 * arguments: none
 *
 * returns: none
 */

void I2C1_Start(void);

/*****************************************************************************/

/*
 * function name: I2C1_Restart
 *
 * description: Sends a repeated start condition to the I2C. Works in the same
 *              way as start_i2c
 *
 * arguments: none
 *
 * returns: none
 */

void I2C1_Restart(void);

/*****************************************************************************/

/*
 * function name: I2C1_Stop
 *
 * description: Sends a stop condition to the I2C. Works in the same way as
 *              start_i2c
 *
 * arguments: none
 *
 * returns: none
 */

void I2C1_Stop(void);

/*****************************************************************************/

/*
 * function name: I2C1_Write
 *
 * description: Writes a byte of data to the I2C TX buffer and waits for an
 *              ACK from the slave before returning
 *
 * arguments: uint8_t, data to be sent by the master
 * 
 * returns: none
 */

void I2C1_Write(uint8_t);

/*****************************************************************************/

/*
 * function name: I2C1_Read
 *
 * description: Reads a byte of data from the I2C RX buffer. Sends a read
 *              enable bit and waiting for the RX buffer to fill with a valid
 *              byte of data
 *
 * arguments: none
 *
 * returns: uint8_t data sent by the slave
 */

uint8_t I2C1_Read(void)

/*****************************************************************************/

/*
 * function name: RTCC_Set
 *
 * description: Sets the time on the RTCC when the clock is first initialised.
 *              Writes to the RTCC automatically advance the register being
 *              written to, so each element of the clock and calendar is sent
 *              consecutively.
 *
 * arguments: none
 *
 * returns: none
 */

void RTCC_Set(void);

/*****************************************************************************/

/*
 * function name: RTCC_Read
 *
 * description: Reads values of the RTCC when called and stores them in their
 *              respective registers. Like as in writing, the addresses of the
 *              registers are automatically advanced to allow for consecutive
 *              reads.
 *
 * arguments:
 *
 * returns:
 */

void RTCC_Read(void);
