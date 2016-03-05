#ifndef _DELAY_H_
#define _DELAY_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/fpu.h"
#include "driverlib/uart.h"


void delay_us(unsigned int n);
void delay_ms(unsigned int n);

#endif /*_DELAY_H_*/
