#include <rtcc.h>

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

void set_rtcc(void){
    /* Writing seconds data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCSEC.ADDR);
    write_i2c(RTCSEC.tens << 4 | RTCSEC.ones);
    stop_i2c();

    /* Writing minutes data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCMIN.ADDR);
    write_i2c(RTCMIN.tens << 4 | RTCMIN.ones);
    stop_i2c();

    /* Writing hours data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCHOUR.ADDR);
    write_i2c((RTCHOUR.format << 2 | RTCHOUR.tens) << 4 | RTCHOUR.ones);
    stop_i2c();

    /* Writing weekday data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCWKDAY.ADDR);
    write_i2c(RTCWKDAY.oscrun << 5 | RTCWKDAY.ones);
    stop_i2c();

    /* Writing date data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCDATE.ADDR);
    write_i2c(RTCDATE.tens << 4 | RTCDATE.ones);
    stop_i2c();

    /* Writing month data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCMTH.ADDR);
    write_i2c((RTCMTH.leapyear << 1 | RTCMTH.tens) << 4 | RTCMTH.ones);
    stop_i2c();

    /* Writing month data to RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCYEAR.ADDR);
    write_i2c(RTCYEAR.tens << 4 | RTCYEAR.ones);
    stop_i2c();
}

void read_rtcc(void){
    uint8_t buf;
    
}