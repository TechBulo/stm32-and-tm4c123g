#ifndef _KEY_H_
#define _KEY_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "inc/hw_gpio.h"

#define GPIO_KEY_CTL		(SYSCTL_PERIPH_GPIOB)
#define GPIO_KEY   			(GPIO_PORTB_BASE)
#define GPIO_Key_0 			(GPIO_PIN_0)
#define GPIO_Key_1 			(GPIO_PIN_1)
//#define GPIO_Key_2 			(GPIO_PIN_2)
//#define GPIO_Key_3 			(GPIO_PIN_3)
#define GPIO_Key_4 			(GPIO_PIN_4)
#define GPIO_Key_5 			(GPIO_PIN_5)
#define GPIO_Key_6 			(GPIO_PIN_6)
#define GPIO_Key_7 			(GPIO_PIN_7)

#define GPIO_ResetBits(x, y)	(GPIOPinWrite(x, y, ~y))
#define GPIO_SetBits(x, y)		(GPIOPinWrite(x, y, y))

extern void KEY_GPIO_Config(void);
extern char ENTER_Key(void);

typedef struct Key_Str
{
    uint32_t GPIO_X;
    uint16_t GPIO_Pin;
}Key_Str;

static Key_Str Key_Out[2] =
{
    {GPIO_KEY,GPIO_Key_0},{GPIO_KEY,GPIO_Key_1},
//    {GPIO_KEY,GPIO_Key_2},{GPIO_KEY,GPIO_Key_3}
};

static Key_Str Key_Int[4] =
{
    {GPIO_KEY,GPIO_Key_4},{GPIO_KEY,GPIO_Key_5},
    {GPIO_KEY,GPIO_Key_6},{GPIO_KEY,GPIO_Key_7}
} ;

static char Key[4][4] =
{
    'D' , 'C', 'B', 'A',
    '#' , '9', '6', '3',
    '0' , '8', '5', '2',
    '*' , '7', '4', '1',
};
#endif /* _KEY_H_ */
