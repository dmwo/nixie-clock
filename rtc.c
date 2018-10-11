#include <rtc.h>
#include <settime.h>

/*******************************************************************************
* File: rtc.c
* Author: Dylan Oh
* Date Created: 23 Dec 2017
* Desc: Contains functions that operate and retrieve data from the real
*       time clock onboard the PIC24HJ128GP504-I/PT for Nixie Tube Clock
*       application.
*******************************************************************************/

void setRTCWREN(void){
    __asm__ ("MOV   #OSCCON, W1");
    __asm__ ("MOV   #0x55, W2");
    __asm__ ("MOV   #0xAA, W3");
    __asm__ ("MOV   W2, [W1]");
    __asm__ ("MOV   W3, [W1]");
    __asm__ ("BSET  RCFGCAL, #13");
}

void clearRTCWREN(void){
    RCFGCALBITS.RTCWREN = 0;
}

void init_RTC(void){
    setRTCWREN();
    RCFGCALBITS.RTCPTR = 0b11;
    RTCVAL = 0x0000;
    RTCVAL = 0x0000;
    RTCVAL = 0x0000;
    RTCVAL = 0x0000;
    clearRTCWREN();
}

void set_Time(timedate_t param){
    setRTCWREN();
    RCFGCALBITS.RTCPTR = 0b01;
    RTCVAL = (param.time[1] << 4) | param.time[2];
    RTCVAL = (((param.time[3]) << 4) | param.time[4]) << 8 | ((param.time[5] << 4) | param.time[6]);
    clearRTCWREN();
}

void set_Date(timedate_t param){
    setRTCWREN();
    RCFGCALBITS.RTCPTR = 0b11;
    RTCVAL = 
    clearRTCWREN();
}

void grab_Time(timedate_t param){
    setRTCWREN();
    RCFGCALBITS.RTCPTR -= 1;
    param.time[1] = (RTCVAL & 0x00F0) >> 4;
    param.time[2] = RTCVAL & 0x000F;
    RCFGCALBITS.RTCPTR -= 1;
    param.time[3] = (RTCVAL & 0xF000) >> 12;
    param.time[4] = (RTCVAL & 0x0F00) >> 8;
    param.time[5] = (RTCVAL & 0x00F0) >> 4;
    param.time[6] = RTCVAL & 0x000F;
    clearRTCWREN();
}

void grab_Date(timedate_t param){
    setRTCWREN();
    RCFGCALBITS.RTCPTR = 0b11;
    param.date[6] = (RTCVAL & 0x00F0) >> 4;
    param.date[5] = RTCVAL & 0x000F;
    RCFGCALBITS.RTCPTR -= 1;
    param.date[3] = (RTCVAL & 0xF000) >> 12;
    param.date[4] = (RTCVAL & 0x0F00) >> 8;
    param.date[1] = (RTCVAL & 0x00F0) >> 4;
    param.date[2] = RTCVAL & 0x000F;
    clearRTCWREN();
}