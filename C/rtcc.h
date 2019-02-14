#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif

#ifndef RTCC_H
#define	RTCC_H

#define _XTAL_FREQ (16000000)
#define CTL_IN_R   (0b11011111)
#define CTL_IN_W   (0b11011110)
#define INPUT      (1)
#define OUTPUT     (0)

/* RTCC timekeeping registers */
// Register: RTCSEC
typedef struct {
    uint8_t ones_BCD;
    uint8_t tens_BCD;
    uint8_t ones_shift;
    uint8_t tens_shift;
    uint8_t st;
    uint8_t ADDR = 0x00;
} RTCSEC_t;
extern volatile RTCSEC_t RTCSEC;

// Register: RTCMIN
typedef struct{
    uint8_t ones_BCD;
    uint8_t tens_BCD;
    uint8_t ones_shift;
    uint8_t tens_shift;
    uint8_t ADDR = 0x01;
};
} RTCMIN_t;
extern volatile RTCMIN_t RTCMIN;

// Register: RTCHOUR
typedef struct{
    uint8_t ones_BCD;
    uint8_t tens_BCD;
    uint8_t ones_shift;
    uint8_t tens_shift;
    uint8_t format;
    uint8_t ADDR = 0x02;
} RTCHOUR_t;
extern volatile RTCHOUR_t RTCHOUR;

// Register: RTCWKDAY
typedef struct{
    uint8_t ones_BCD;
    uint8_t ones_shift;
    uint8_t oscrun;
    uint8_t ADDR = 0x03;
} RTCWKDAY_t;
extern volatile RTCWKDAY_t RTCWKDAY;

// Register: RTCDATE
typedef struct{
    uint8_t ones_BCD;
    uint8_t tens_BCD;
    uint8_t ones_shift;
    uint8_t tens_shift;
    uint8_t ADDRESS = 0x04;
} RTCDATE_t;
extern volatile RTCDATE_t RTCDATE;

// Register: RTCMTH
typedef struct{
    uint8_t ones_BCD;
    uint8_t tens_BCD;
    uint8_t ones_shift;
    uint8_t tens_shift;
    uint8_t leapyear;
    uint8_t ADDR = 0x05;
} RTCMTH_t;
extern volatile RTCMTH_t RTCMTH;

// Register: RTCYEAR
typedef struct{
    uint8_t ones_BCD;
    uint8_t tens_BCD;
    uint8_t ones_shift;
    uint8_t tens_shift;
    uint8_t ADDR = 0x06;
} RTCDATE_t;
extern volatile RTCDATE_t RTCDATE;

/* Function Declarations */
void init_i2c(int baud);
void wait_i2c(void);
void start_i2c(void);
void restart_i2c(void);
void stop_i2c(void);
void write_i2c(uint8_t data);
void set_rtcc(void);
void read_rtcc(void);