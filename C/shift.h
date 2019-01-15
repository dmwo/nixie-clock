#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif

#ifndef SHIFT_H
#define	SHIFT_H

/* Function Declarations */
void init_spi(void);
void wait_spi(void);
void write_spi(uint8_t data);
void write_shift(void);