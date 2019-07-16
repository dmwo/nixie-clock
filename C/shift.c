#include "shift.h"

/******************************************************************************
 * filename: shift.c                                                          *
 *                                                                            *
 * purpose: Contains functions used for operating the three SN74HC595N shift  *
 *          registers used to encode and set the K155ID1 nixie tube drivers   *
 *          with BCD values using SPI.                                        *
 *                                                                            *
 * date created: 28 Dec 2018                                                  *
 *                                                                            *
 * author: Dylan Oh                                                           *
 *****************************************************************************/
 
void SPI2_Init(void){
    MSSP2_SPI_Enable();        // Enable master synchronous serial port 2
    SPI2_Master_Mode();        // Set MSSP2 as SPI master mode
    SPI2_Clear_Status();       // Clear all SPI statuses
}

void SPI2_Write(uint8_t data){
    SPI2_Write_Clear();        // Clear write collision flag to allow writing
    SSP2BUF = data;            // Write data to SPI buffer
    SPI2_Wait_Buffer();        // Wait for buffer to fill with receive data
    data = SSP2BUF;            // Read garbage data from slave to clear buffer
}

void SPI2_Wait_Buffer(void){
    while(!SPI_Status_Buffer); // Wait for the buffer to empty (write complete)
}

void SN74HC595_Write(void){
    if (mode == TIME || mode == TIMESEL){
        SPI2_Write(lookup_ones[secRTC.ones]   << 4 | lookup_tens[secRTC.tens]);
        SPI2_Write(lookup_ones[minRTC.ones]   << 4 | lookup_tens[minRTC.tens]);
        SPI2_Write(lookup_ones[hourRTC.ones]  << 4 | lookup_tens[hourRTC.tens]);
    } else if (mode == DATE || mode == DATESEL){
        SPI2_Write(lookup_ones[yearRTC.ones]  << 4 | lookup_tens[yearRTC.tens]);
        SPI2_Write(lookup_ones[monthRTC.ones] << 4 | lookup_tens[monthRTC.tens]);
        SPI2_Write(lookup_ones[dateRTC.ones]  << 4 | lookup_tens[dateRTC.tens]);
    } else {
        SPI2_Write(lookup_ones[wkdayRTC.ones] << 4 | lookup_tens[0]);
        SPI2_Write(lookup_ones[0] << 4 | lookup_tens[0]);
        SPI2_Write(lookup_ones[0] << 4 | lookup_tens[0]);
    }
}