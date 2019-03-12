#include "rtcc.h"

/******************************************************************************
 * filename: rtcc.c                                                           *
 *                                                                            *
 * purpose: Contains functions that communicate with the MCP7940M real time   *
 *          clock and calendar using I2C.                                     *
 *                                                                            *
 * date created: 28 Dec 2018                                                  *
 *                                                                            *
 * author: Dylan Oh                                                           *
 *****************************************************************************/

/******************************************************************************
 * FUNCTION DEFINITIONS                                                       *
 *****************************************************************************/

void I2C1_Init(uint8_t baud){
    TRISBbits.TRISB5 = INPUT;  // Set pin B5 as SCL input
    TRISBbits.TRISB7 = INPUT;  // Set pin B7 as SDA input
    MSSP1_I2C_Enable();        // Enable master synchronous serial port
    I2C1_Master_Mode();        // Set I2C master mode
    SSP2CON2 = 0;              // Reset I2C
    SSP2ADD = (_XTAL_FREQ / 4 * baud)) - 1
    I2C1_Clear_Status();
}

void I2C1_Wait(void){
    while(SSP2STAT.R_nW || I2C_Status_ACK);
}

void I2C1_Start(void){
    I2C_Send_Start();          // Send start condition
    while(I2C_Status_Start);   // Wait for successful start
    I2C1_Wait();
}

void I2C1_Restart(void){
    I2C_Send_Restart();        // Send repeated start condition
    while(I2C_Status_Restart); // Wait for successful restart
    I2C1_Wait();
}

void I2C1_Stop(void){
    I2C_Send_Stop();           // Send stop condition
    while(I2C_Status_Stop);    // Wait for successful stop
    I2C1_Wait();
}

void I2C1_Write(uint8_t data){
    SSP2BUF = data;
    I2C1_Wait();
}

uint8_t I2C1_Read(void){
    uint8_t data;
    RCEN = 1;
    I2C1_Wait();
    data = SSPBUF;
    I2C1_Wait();
    ACKDT = 
}

void RTCC_Set(void){
    I2C1_Start();
    I2C1_Write(CTL_IN_W);
    I2C1_Write(SEC_ADDR);
    I2C1_Write(secRTC.tens << 4 | secRTC.ones);
    I2C1_Write(minRTC.tens << 4 | minRTC.ones);
    I2C1_Write((hourRTC.extra << 2 | hourRTC.tens) << 4 | hourRTC.ones);
    I2C1_Write(wkdayRTC.extra << 5 | wkdayRTC.ones);
    I2C1_Write(dateRTC.tens << 4 | dateRTC.ones);
    I2C1_Write((monthRTC.extra << 1 | monthRTC.tens) << 4 | monthRTC.ones);
    I2C1_Write(yearRTC.tens << 4 | yearRTC.ones);
    I2C1_Stop();
}

void RTCC_Read(void){
    uint8_t data;

    /* Reading seconds data from RTCC */
    I2C1_Start();
    I2C1_Write(CTL_IN_W);
    I2C1_Write(RTCSEC.ADDR);
    I2C1_Restart();
    I2C1_Write(CTL_IN_R);

    data = I2C1_Read();
    secRTC.extra   = (data & 0b10000000) >> 7;
    secRTC.tens    = (data & 0b01110000) >> 4;
    secRTC.ones    =  data & 0b00001111;

    data = I2C1_Read();
    minRTC.tens    = (data & 0b01110000) >> 4;
    minRTC.ones    =  data & 0b00001111;

    data = I2C1_Read();
    hourRTC.extra  = (data & 0b01000000) >> 6;
    hourRTC.tens   = (data & 0b00110000) >> 4;
    hourRTC.ones   =  data & 0b00001111;

    data = I2C1_Read();
    wkdayRTC.extra = (data & 0b00100000) >> 5;
    wkdayRTC.ones  =  data & 0b00000111;

    data = I2C1_Read();
    dateRTC.tens   = (data & 0b00110000) >> 4;
    dateRTC.ones   =  data & 0b00001111;

    data = I2C1_Read();
    monthRTC.extra = (data & 0b00100000) >> 5;
    monthRTC.tens  = (data & 0b00010000) >> 4;
    monthRTC.ones  =  data & 0b00001111;

    data = I2C1_Read();
    yearRTC.tens   = (data & 0b11110000) >> 4;
    yearRTC.ones   =  data & 0b00001111;

    I2C1_Stop();
}