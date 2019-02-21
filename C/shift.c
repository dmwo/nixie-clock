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

// fix in eagle: pin A => QA and QE
//               pin B => QB and QF
//               pin C => QC and QG
//               pin D => QD and QH
// QABCD correspond to higher place
// QEFGH correspond to lower place
void write_shift(uint8_t mode){
    if (mode == TIME){
        write_spi(secRTC.ones_BCD  << 4 | secRTC.tens_BCD);
        write_spi(minRTC.ones_BCD  << 4 | minRTC.tens_BCD);
        write_spi(hourRTC.ones_BCD << 4 | hourRTC.tens_BCD);
    } else if (mode == DATE){
        write_spi(yearRTC.ones_BCD  << 4 | yearRTC.tens_BCD);
        write_spi(monthRTC.ones_BCD << 4 | monthRTC.tens_BCD);
        write_spi(dateRTC.ones_BCD  << 4 | dateRTC.tens_BCD);
    }
}