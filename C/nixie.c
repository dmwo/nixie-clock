#include "nixie.h"

/******************************************************************************
 * filename: nixie.c                                                          *
 *                                                                            *
 * purpose: Contains functions pertaining to the operation of the 
 *                                                                            *
 * date created: 1 July 2019                                                  *
 *                                                                            *
 * authors: Dylan Oh                                                          *
 *****************************************************************************/

/******************************************************************************
 * FUNCTION DEFINITIONS                                                       *
 *****************************************************************************/

uint8_t convert_BCD(uint8_t val, bool dir){
    if (dir == BCD_TO_DEC) return ((val >> 4) * 10 + (val & 0xF));
    else return (((val / 10) << 4) | (val % 10));
}

void set_nixie(void){
    /* Setting tens place digits */
    dateRTC.tens  = (convert_BCD(num[DAYSEL], DEC_TO_BCD) & 0xF0) >> 4;
    monthRTC.tens = (convert_BCD(num[MONSEL], DEC_TO_BCD) & 0xF0) >> 4;
    yearRTC.tens  = (convert_BCD(num[YRSEL],  DEC_TO_BCD) & 0xF0) >> 4;
    secRTC.tens   = (convert_BCD(num[SECSEL], DEC_TO_BCD) & 0xF0) >> 4;
    minRTC.tens   = (convert_BCD(num[MINSEL], DEC_TO_BCD) & 0xF0) >> 4;
    hourRTC.tens  = (convert_BCD(num[HRSEL],  DEC_TO_BCD) & 0xF0) >> 4;

    /* Setting ones place digits */
    dateRTC.ones  = (convert_BCD(num[DAYSEL], DEC_TO_BCD) & 0x0F);
    monthRTC.ones = (convert_BCD(num[MONSEL], DEC_TO_BCD) & 0x0F);
    yearRTC.ones  = (convert_BCD(num[YRSEL],  DEC_TO_BCD) & 0x0F);
    secRTC.ones   = (convert_BCD(num[SECSEL], DEC_TO_BCD) & 0x0F);
    minRTC.ones   = (convert_BCD(num[MINSEL], DEC_TO_BCD) & 0x0F);
    hourRTC.ones  = (convert_BCD(num[HRSEL],  DEC_TO_BCD) & 0x0F);

    /* Setting weekday */
    wkdayRTC.ones = num[WKDSEL];
}

void nixie_toggle(void){
    SW_HRDAY = SW_MINMON = SW_SECYR = true;
    for (int i = 0; i < DELAY_TICK; i++);
    if      (digit == DAYSEL || digit == HRSEL)  SW_HRDAY = false;
    else if (digit == MONSEL || digit == MINSEL) SW_MINMON = false;
    else if (digit == YRSEL  || digit == SECSEL) SW_SECYR = false;
    for (int i = 0; i < DELAY_TICK; i++);
}