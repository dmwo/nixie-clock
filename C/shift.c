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
    SSP1CON1bits.SSPM = 0;     // Set SPI master mode
    SSP1CON2 = 0;              // Reset SPI
    SSPSTAT = 0;    
}

void wait_spi(void){
    while(!SSPSTATbits.BF);
}

void write_spi(uint8_t data){
    uint8_t temp;
    SSP1BUF = data;
    wait_spi();
    temp = SSP1BUF;
}

void write_shift(int mode){
    if (mode == TIME){
        write_spi(secRTC.tens_shift >> 4 | secRTC.ones_shift);
        write_spi(minRTC.tens_shift >> 4 | minRTC.ones_shift);
        write_spi(hourRTC.tens_shift >> 4 | hourRTC.ones_shift);
    } else if (mode == DATE){
        write_spi(yearRTC.tens_shift >> 4 | yearRTC.ones_shift);
        write_spi(monthRTC.tens_shift >> 4 | monthRTC.ones_shift);
        write_spi(dateRTC.tens_shift >> 4 | dateRTC.ones_shift);
    }
}