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
    SSP2CON1bits.SSPEN = 1;    // Enable master synchronous serial port
    SSP2CON1bits.SSPM = 0;     // Set SPI master mode
    SSP2CON2 = 0;              // Reset SPI
    SSP2STAT = 0;    
}

void write_spi(uint8_t data){
    SPI_Write_Clear();  // Clear write collision flag to allow writing
    SSP2BUF = data;     // Write data to SPI buffer
    SPI_Wait_Buffer();  // Wait for buffer to empty
    data = SSP2BUF;     // Read garbage data from slave to clear buffer
}

void write_shift(){
    if (param.mode == TIME || param.mode == TIMESELECT){
        write_spi(lookup_ones[secRTC.ones]  << 4 | lookup_tens[secRTC.tens)];
        write_spi(lookup_ones[minRTC.ones]  << 4 | lookup_tens[minRTC.tens)];
        write_spi(lookup_ones[hourRTC.ones] << 4 | lookup_tens[hourRTC.tens)];
    } else if (param.mode == DATE || param.mode == DATESELECT){
        write_spi(lookup_ones[yearRTC.ones]  << 4 | lookup_tens[yearRTC.tens)];
        write_spi(lookup_ones[monthRTC.ones] << 4 | lookup_tens[monthRTC.tens)];
        write_spi(lookup_ones[dateRTC.ones]  << 4 | lookup_tens[dateRTC.tens)];
    }
}