#include <xc.h>
#include "shift.h"

/*******************************************************************************
* File: shift.c
* Author: Dylan Oh
* Date Created: 28 Dec 2018
* Desc: Contains functions that communicate with the SN74HC595N shift registers
        using SPI.
*******************************************************************************/

void init_spi(void){
    TRISBbits.TRISB6 = OUTPUT; // Set pin B6 as SCK input
    TRISAbits.TRISA2 = OUTPUT; // Set pin A2 as SDO input
    SSP1CON1bits.SSPEN = 1;    // Enable master synchronous serial port
    SSP2CON1bits.SSPM = 0;     // Set SPI master mode
    SSP2CON2 = 0;              // Reset SPI
    SSP2STAT = 0;    
}

void wait_spi(void){
    while(!SSPSTATbits.BF);
}

void write_spi(uint8_t data){
    SSP2CON1bits.WCOL = 0; // clear write collision flag to allow writing
    SSP2BUF = data;
    wait_spi();
    uint8_t garbage = SSP1BUF;
}

void write_shift(){
    if (param.mode == TIME || param.mode == TIMESELECT){
        write_spi(lookup[secRTC.ones]  << 4 | lookup[secRTC.tens)];
        write_spi(lookup[minRTC.ones]  << 4 | lookup[minRTC.tens)];
        write_spi(lookup[hourRTC.ones] << 4 | lookup[hourRTC.tens)];
    } else if (param.mode == DATE || param.mode == DATESELECT){
        write_spi(lookup[yearRTC.ones]  << 4 | lookup[yearRTC.tens)];
        write_spi(lookup[monthRTC.ones] << 4 | lookup[monthRTC.tens)];
        write_spi(lookup[dateRTC.ones]  << 4 | lookup[dateRTC.tens)];
    }
}