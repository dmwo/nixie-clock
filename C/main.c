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

init_GPIO();
init_spi();
init_i2c();

int main(int argc, char** argv) {
    return (EXIT_SUCCESS);
}
