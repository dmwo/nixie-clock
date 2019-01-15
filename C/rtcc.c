#include "rtcc.h"

/*******************************************************************************
* File: rtcc.c
* Author: Dylan Oh
* Date Created: 28 Dec 2018
* Desc: Contains functions that communicate with the MCP7940M real time clock
        and calendar using I2C.
*******************************************************************************/

void init_i2c(int baud){
    TRISBbits.TRISB5 = INPUT; // Set pin B5 as SCL input
    TRISBbits.TRISB7 = INPUT; // Set pin B7 as SDA input
    SSP2CON1bits.SSPEN = 1;   // Enable master synchronous serial port
    SSP2CON1bits.SSPM = 0x8;  // Set I2C master mode
    SSP2CON2 = 0;             // Reset I2C
    SSP2ADD = (_XTAL_FREQ / 4 * baud)) - 1
    SSPSTAT = 0;
}

void wait_i2c(void){
    /* 
     * While R_nW is high (data is being transmitted over the I2C bus) OR
     * ACKSTAT is high (either NACK or no ACK), do nothing.
     */

    while(SSP2STAT.R_nW || SSP2CON2bits.ACKSTAT);
}

void start_i2c(void){
    SSP2CON2bits.SEN = 1;     // Send start condition
    while(SSP2CON2bits.SEN);  // Wait for successful start
    wait_i2c();
}

void restart_i2c(void){
    SSP2CON2bits.RSEN = 1;    // Send repeated start condition
    while(SSP2CON2bits.RSEN); // Wait for successful restart
    wait_i2c();
}

void stop_i2c(void){
    SSP2CON2bits.PEN = 1;     // Send stop condition
    while(SSP2CON2bits.PEN);  // Wait for successful stop
    wait_i2c();
}

void write_i2c(uint8_t data){
    SSP2BUF = data;
    wait_i2c();
}

uint8_t read_i2c(){
    uint8_t buf;
    RCEN = 1;
    wait_i2c();
    buf = SSPBUF;
    wait_i2c();
    ACKDT = 
}

void shift_BCD(void){
    RTCSEC.ones_shift[0] = RTCSEC.ones_BCD[2]; RTCSEC.ones_shift[1] = RTCSEC.ones_BCD[1];
    RTCSEC.ones_shift[2] = RTCSEC.ones_BCD[3]; RTCSEC.ones_shift[3] = RTCSEC.ones_BCD[0];
    RTCSEC.tens_shift[0] = RTCSEC.tens_BCD[2]; RTCSEC.tens_shift[1] = RTCSEC.tens_BCD[1];
    RTCSEC.tens_shift[2] = RTCSEC.tens_BCD[3]; RTCSEC.tens_shift[3] = RTCSEC.tens_BCD[0];

    RTCMIN.ones_shift[0] = RTCMIN.ones_BCD[2]; RTCMIN.ones_shift[1] = RTCMIN.ones_BCD[1];
    RTCMIN.ones_shift[2] = RTCMIN.ones_BCD[3]; RTCMIN.ones_shift[3] = RTCMIN.ones_BCD[0];
    RTCMIN.tens_shift[0] = RTCMIN.tens_BCD[2]; RTCMIN.tens_shift[1] = RTCMIN.tens_BCD[1];
    RTCMIN.tens_shift[2] = RTCMIN.tens_BCD[3]; RTCMIN.tens_shift[3] = RTCMIN.tens_BCD[0];

    RTCHOUR.ones_shift[0] = RTCHOUR.ones_BCD[2]; RTCHOUR.ones_shift[1] = RTCHOUR.ones_BCD[1];
    RTCHOUR.ones_shift[2] = RTCHOUR.ones_BCD[3]; RTCHOUR.ones_shift[3] = RTCHOUR.ones_BCD[0];
    RTCHOUR.tens_shift[0] = RTCHOUR.tens_BCD[2]; RTCHOUR.tens_shift[1] = RTCHOUR.tens_BCD[1];
    RTCHOUR.tens_shift[2] = RTCHOUR.tens_BCD[3]; RTCHOUR.tens_shift[3] = RTCHOUR.tens_BCD[0];

    RTCWKDAY.ones_shift[0] = RTCWKDAY.ones_BCD[2]; RTCWKDAY.ones_shift[1] = RTCWKDAY.ones_BCD[1];
    RTCWKDAY.ones_shift[2] = RTCWKDAY.ones_BCD[3]; RTCWKDAY.ones_shift[3] = RTCWKDAY.ones_BCD[0];

    RTCDATE.ones_shift[0] = RTCDATE.ones_BCD[2]; RTCDATE.ones_shift[1] = RTCDATE.ones_BCD[1];
    RTCDATE.ones_shift[2] = RTCDATE.ones_BCD[3]; RTCDATE.ones_shift[3] = RTCDATE.ones_BCD[0];
    RTCDATE.tens_shift[0] = RTCDATE.tens_BCD[2]; RTCDATE.tens_shift[1] = RTCDATE.tens_BCD[1];
    RTCDATE.tens_shift[2] = RTCDATE.tens_BCD[3]; RTCDATE.tens_shift[3] = RTCDATE.tens_BCD[0];

    RTCMTH.ones_shift[0] = RTCMTH.ones_BCD[2]; RTCMTH.ones_shift[1] = RTCMTH.ones_BCD[1];
    RTCMTH.ones_shift[2] = RTCMTH.ones_BCD[3]; RTCMTH.ones_shift[3] = RTCMTH.ones_BCD[0];
    RTCMTH.tens_shift[0] = RTCMTH.tens_BCD[2]; RTCMTH.tens_shift[1] = RTCMTH.tens_BCD[1];
    RTCMTH.tens_shift[2] = RTCMTH.tens_BCD[3]; RTCMTH.tens_shift[3] = RTCMTH.tens_BCD[0];

    RTCYEAR.ones_shift[0] = RTCYEAR.ones_BCD[2]; RTCYEAR.ones_shift[1] = RTCYEAR.ones_BCD[1];
    RTCYEAR.ones_shift[2] = RTCYEAR.ones_BCD[3]; RTCYEAR.ones_shift[3] = RTCYEAR.ones_BCD[0];
    RTCYEAR.tens_shift[0] = RTCYEAR.tens_BCD[2]; RTCYEAR.tens_shift[1] = RTCYEAR.tens_BCD[1];
    RTCYEAR.tens_shift[2] = RTCYEAR.tens_BCD[3]; RTCYEAR.tens_shift[3] = RTCYEAR.tens_BCD[0];
}

void set_rtcc(void){
    /* Writing seconds data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCSEC.ADDR);
    write_i2c(RTCSEC.tens_BCD << 4 | RTCSEC.ones_BCD);
    stop_i2c();

    /* Writing minutes data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCMIN.ADDR);
    write_i2c(RTCMIN.tens_BCD << 4 | RTCMIN.ones_BCD);
    stop_i2c();

    /* Writing hours data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCHOUR.ADDR);
    write_i2c((RTCHOUR.format << 2 | RTCHOUR.tens_BCD) << 4 | RTCHOUR.ones_BCD);
    stop_i2c();

    /* Writing weekday data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCWKDAY.ADDR);
    write_i2c(RTCWKDAY.oscrun << 5 | RTCWKDAY.ones_BCD);
    stop_i2c();

    /* Writing date data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCDATE.ADDR);
    write_i2c(RTCDATE.tens_BCD << 4 | RTCDATE.ones_BCD);
    stop_i2c();

    /* Writing month data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCMTH.ADDR);
    write_i2c((RTCMTH.leapyear << 1 | RTCMTH.tens_BCD) << 4 | RTCMTH.ones_BCD);
    stop_i2c();

    /* Writing month data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCYEAR.ADDR);
    write_i2c(RTCYEAR.tens_BCD << 4 | RTCYEAR.ones_BCD);
    stop_i2c();
}

void read_rtcc(void){
    uint8_t data;

    /* 
     * Reading seconds data from RTCC
     */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCSEC.ADDR);
    restart_i2c();
    write_i2c(CTL_IN_R);
    data = read_i2c();
    stop_i2c();
    /* Storing information in RTCSEC struct */
    RTCSEC.st       = (data & 0b10000000) >> 7;
    RTCSEC.tens_BCD = (data & 0b01110000) >> 4;
    RTCSEC.ones_BCD = data & 0b00001111;

    /* 
     * Reading minutes data from RTCC
     */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCMIN.ADDR);
    restart_i2c();
    write_i2c(CTL_IN_R);
    data = read_i2c();
    stop_i2c();
    /* Storing information in RTCMIN struct */
    RTCMIN.tens_BCD = (data & 0b01110000) >> 4;
    RTCMIN.ones_BCD = data & 0b00001111;

    /* 
     * Reading hours data from RTCC
     */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCHOUR.ADDR);
    restart_i2c();
    write_i2c(CTL_IN_R);
    data = read_i2c();
    stop_i2c();
    /* Storing information in RTCHOUR struct */
    RTCHOUR.format   = (data & 0b01000000) >> 6;
    RTCHOUR.tens_BCD = (data & 0b00110000) >> 4;
    RTCHOUR.ones_BCD = data & 0b00001111;

    /* 
     * Reading weekday data from RTCC
     */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCWKDAY.ADDR);
    restart_i2c();
    write_i2c(CTL_IN_R);
    data = read_i2c();
    stop_i2c();
    /* Storing information in RTCWKDAY struct */
    RTCWKDAY.format   = (data & 0b00100000) >> 5;
    RTCWKDAY.ones_BCD = data & 0b00000111;

    /* 
     * Reading date data from RTCC
     */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCDATE.ADDR);
    restart_i2c();
    write_i2c(CTL_IN_R);
    data = read_i2c();
    stop_i2c();
    /* Storing information in RTCDATE struct */
    RTCDATE.tens_BCD = (data & 0b00110000) >> 4;
    RTCDATE.ones_BCD = data & 0b00001111;

    /* 
     * Reading month data from RTCC
     */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCMTH.ADDR);
    restart_i2c();
    write_i2c(CTL_IN_R);
    data = read_i2c();
    stop_i2c();
    /* Storing information in RTCMTH struct */
    RTCMTH.format   = (data & 0b00100000) >> 5;
    RTCMTH.tens_BCD = (data & 0b00010000) >> 4;
    RTCMTH.ones_BCD = data & 0b00001111;

    /* 
     * Reading year data from RTCC
     */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCYEAR.ADDR);
    restart_i2c();
    write_i2c(CTL_IN_R);
    data = read_i2c();
    stop_i2c();
    /* Storing information in RTCYEAR struct */
    RTCYEAR.tens_BCD = (data & 0b11110000) >> 4;
    RTCYEAR.ones_BCD = data & 0b00001111;
}