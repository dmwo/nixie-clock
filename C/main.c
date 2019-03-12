/* 
 * File:   main.c
 * Author: Dylan
 *
 * Created on 23 December 2017, 16:07
 */

#include "nixie.h"
#include "rtcc.h"
#include "shift.h"

/*
 * 
 */

void main(void) {
    /* Enabling peripherals */
    GPIO_Init();
    SPI2_Init();
    // I2C1_Init();
}
