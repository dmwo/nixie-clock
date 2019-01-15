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
    SSP1CON1bits.SSPEN = 1;   // Enable master synchronous serial port
    SSP1CON1bits.SSPM = 0xA;  // Set SPI master mode
    SSP1CON2 = 0;             // Reset SPI
    SSP1ADD = (_XTAL_FREQ / 4 * baud)) - 1
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