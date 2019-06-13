/* 
 * File:   main.c
 * Author: Dylan
 *
 * Created on 23 December 2017, 16:07
 */

#include "gpio.h"
#include "rtcc.h"
#include "shift.h"
#include "timer.h"

/*
 * 
 */

void main(void) {
    /* Enabling peripherals */
    GPIO_Init();
    SPI2_Init();
    Interrupt_Init();
    // I2C1_Init();

    Global_Int_Enable();
    Peripheral_Int_Enable();

    while (mode != TIME) nixie_toggle();

    while (1){

    }
}
