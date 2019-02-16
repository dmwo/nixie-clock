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

/*
 * function name: init_i2c
 *
 * description: Initialises the I2C by:
 *              - setting the SCL and SDA ports in peripheral pin select
 *              - enabling MSSP1
 *              - setting I2C mode to master
 *              - setting the baud rate
 *
 * arguments:
 * argument     type        description
 * --------     ----        -----------
 * baud         uint8_t     desired baud rate of the I2C
 * 
 * returns: none
 */

void init_i2c(uint8_t baud){
    TRISBbits.TRISB5 = INPUT; // Set pin B5 as SCL input
    TRISBbits.TRISB7 = INPUT; // Set pin B7 as SDA input
    SSP2CON1bits.SSPEN = 1;   // Enable master synchronous serial port
    SSP2CON1bits.SSPM = 0x8;  // Set I2C master mode
    SSP2CON2 = 0;             // Reset I2C
    SSP2ADD = (_XTAL_FREQ / 4 * baud)) - 1
    SSPSTAT = 0;
}

/*****************************************************************************/

/*
 * function name: wait_i2c
 *
 * description: Waits for an ACK from a slave device by polling if data is
 *              being transmitted over the I2C bus or if there is either a
 *              NACK or no ACK
 *
 * arguments: none
 *
 * returns: none
 */

void wait_i2c(void){
    while(SSP2STAT.R_nW || SSP2CON2bits.ACKSTAT);
}

/*****************************************************************************/

/*
 * function name: start_i2c
 *
 * description: Sends a start condition to the I2C. The start enable bit is
 *              automatically cleared by the PIC16F18445, so when that bit is
 *              deasserted, the start condition is successful
 *
 * arguments: none
 *
 * returns: none
 */

void start_i2c(void){
    SSP2CON2bits.SEN = 1;     // Send start condition
    while(SSP2CON2bits.SEN);  // Wait for successful start
    wait_i2c();
}

/*****************************************************************************/

/*
 * function name: restart_i2c
 *
 * description: Sends a repeated start condition to the I2C. Works in the same
 *              way as start_i2c
 *
 * arguments: none
 *
 * returns: none
 */

void restart_i2c(void){
    SSP2CON2bits.RSEN = 1;    // Send repeated start condition
    while(SSP2CON2bits.RSEN); // Wait for successful restart
    wait_i2c();
}

/*****************************************************************************/

/*
 * function name: stop_i2c
 *
 * description: Sends a stop condition to the I2C. Works in the same way as
 *              start_i2c
 *
 * arguments: none
 *
 * returns: none
 */

void stop_i2c(void){
    SSP2CON2bits.PEN = 1;     // Send stop condition
    while(SSP2CON2bits.PEN);  // Wait for successful stop
    wait_i2c();
}

/*****************************************************************************/

/*
 * function name: write_i2c
 *
 * description: Writes a byte of data to the I2C TX buffer and waits for an
 *              ACK from the slave before returning
 *
 * arguments:
 * argument     type        description
 * --------     ----        -----------
 * data         uint8_t     data to be sent by the master
 * 
 * returns: none
 */

void write_i2c(uint8_t data){
    SSP2BUF = data;
    wait_i2c();
}

/*****************************************************************************/

/*
 * function name: read_i2c
 *
 * description: Reads a byte of data from the I2C RX buffer. Sends a read
 *              enable bit and waiting for the RX buffer to fill with a valid
 *              byte of data
 *
 * arguments: none
 *
 * returns: uint8_t data sent by the slave
 */

uint8_t read_i2c(void){
    uint8_t buf;
    RCEN = 1;
    wait_i2c();
    buf = SSPBUF;
    wait_i2c();
    ACKDT = 
}

/*****************************************************************************/

/*
 * function name: set_rtcc
 *
 * description: Sets the time on the RTC when the clock is first initialised.
 *              Writes to the RTC automatically advance the register being
 *              written to, so each element of the clock and calendar is sent
 *              consecutively
 *
 * arguments: none
 *
 * returns: none
 */

void set_rtcc(void){
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(SEC_ADDR);
    write_i2c(secRTC.tens_BCD << 4 | secRTC.ones_BCD);
    write_i2c(minRTC.tens_BCD << 4 | minRTC.ones_BCD);
    write_i2c((hourRTC.format << 2 | hourRTC.tens_BCD) << 4 | hourRTC.ones_BCD);
    write_i2c(wkdayRTC.oscrun << 5 | wkdayRTC.ones_BCD);
    write_i2c(dateRTC.tens_BCD << 4 | dateRTC.ones_BCD);
    write_i2c((monthRTC.leapyear << 1 | monthRTC.tens_BCD) << 4 | monthRTC.ones_BCD);
    write_i2c(yearRTC.tens_BCD << 4 | yearRTC.ones_BCD);
    stop_i2c();
}

/*****************************************************************************/

/*
 * function name: read_rtcc
 *
 * description: 
 *
 * arguments:
 *
 * returns:
 */

void read_rtcc(void){
    uint8_t data;

    /* Reading seconds data from RTCC */
    start_i2c();
    write_i2c(CTL_IN_W);
    write_i2c(RTCSEC.ADDR);
    restart_i2c();
    write_i2c(CTL_IN_R);

    data = read_i2c();
    secRTC.st       = (data & 0b10000000) >> 7;
    secRTC.tens_BCD = (data & 0b01110000) >> 4;
    secRTC.ones_BCD = data & 0b00001111;

    data = read_i2c();
    minRTC.tens_BCD = (data & 0b01110000) >> 4;
    minRTC.ones_BCD = data & 0b00001111;

    data = read_i2c();
    hourRTC.format   = (data & 0b01000000) >> 6;
    hourRTC.tens_BCD = (data & 0b00110000) >> 4;
    hourRTC.ones_BCD = data & 0b00001111;

    data = read_i2c();
    wkdayRTC.format   = (data & 0b00100000) >> 5;
    wkdayRTC.ones_BCD = data & 0b00000111;

    data = read_i2c();
    dateRTC.tens_BCD = (data & 0b00110000) >> 4;
    dateRTC.ones_BCD = data & 0b00001111;

    data = read_i2c();
    monthRTC.format   = (data & 0b00100000) >> 5;
    monthRTC.tens_BCD = (data & 0b00010000) >> 4;
    monthRTC.ones_BCD = data & 0b00001111;

    data = read_i2c();
    yearRTC.tens_BCD = (data & 0b11110000) >> 4;
    yearRTC.ones_BCD = data & 0b00001111;
}