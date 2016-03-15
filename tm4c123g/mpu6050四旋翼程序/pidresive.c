#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "My_uart.h"
#include "pidrevise.h"

void ProData(double *propa, double *intea, double *diffa, double *propb, double *inteb, double *diffb)
{
	char dat[100] = {0};
	int i = 0;
	int k = 0;
	char temp;
	char realdat[100] = {0};

	dat[0] = '@';
	i++;
	while(dat[i-1] != '\r')
	{
		temp = UARTCharGet(UART0_BASE);
		dat[i] = temp;
		i++;
	}
	dat[i-1] = 0;
	//UARTprintf("%s", dat);
	if (dat[1] == 'P' && dat[5] == 'A')
	{
		for (k = 0; dat[k+6] != 0; k++)
			realdat[k] = dat[k+6];
		*propa = atof(realdat);
	}
	else if (dat[1] == 'I' && dat[5] == 'A')
	{
		for (k = 0; dat[k+6] != 0; k++)
			realdat[k] = dat[k+6];
		*intea = atof(realdat);
	}
	else if (dat[1] == 'D' && dat[5] == 'A')
	{
		for (k = 0; dat[k+6] != 0; k++)
			realdat[k] = dat[k+6];
		*diffa = atof(realdat);
	}
	else if (dat[1] == 'P' && dat[5] == 'B')
	{
		for (k = 0; dat[k+6] != 0; k++)
			realdat[k] = dat[k+6];
		*propb = atof(realdat);
	}
	else if (dat[1] == 'I' && dat[5] == 'B')
	{
		for (k = 0; dat[k+6] != 0; k++)
			realdat[k] = dat[k+6];
		*inteb = atof(realdat);
	}
	else if (dat[1] == 'D' && dat[5] == 'B')
	{
		for (k = 0; dat[k+6] != 0; k++)
			realdat[k] = dat[k+6];
		*diffb = atof(realdat);
	}
}


//使用函数格式为 “@PROP值”“@INTE值”“@DIFF值”

void GetData(double *propa, double *intea, double *diffa, double *propb, double *inteb, double *diffb)
{
	char temp;

	while(1)
	{
		temp = UARTCharGet(UART0_BASE);
		if (temp == '@')
		{
			ProData(propa, intea, diffa, propb, inteb, diffb);
			break;
		}
	}
}
