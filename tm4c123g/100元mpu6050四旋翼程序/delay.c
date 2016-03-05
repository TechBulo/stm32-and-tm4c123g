#include "delay.h"

void delay_us(unsigned int n)
{
	unsigned int i = 0;
	for(i = n;i > 0;i--)
		SysCtlDelay(SysCtlClockGet() / 3000000);
}

void delay_ms(unsigned int n)
{
	unsigned int i = 0;
	for(i = n;i > 0;i--)
		SysCtlDelay(SysCtlClockGet() / 3000);
}
