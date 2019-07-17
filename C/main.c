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
#include "nixie.h"

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

    /* Setting the clock; nixies toggle every half second */
    while (mode != TIMEDISP || mode != DATEDISP) nixie_toggle();

    /* Normal operation; time running */
    while (1){
        for (int i = 0; i < _XTAL_FREQ * 10; i++);
        sleep = true;
        EMU_Sleep();
    }
}
