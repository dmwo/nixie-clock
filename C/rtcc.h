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
void init_i2c(int baud);
void wait_i2c(void);
void start_i2c(void);
void restart_i2c(void);
void stop_i2c(void);
void write_i2c(uint8_t data);
void set_rtcc(void);
void read_rtcc(void);
