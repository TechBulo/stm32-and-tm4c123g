#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

uint8_t ui8PrevCount = 2;

void LCD12864_Delay(int n)
{
    int i = 0;
    for(i = n;i > 0;i--)
            SysCtlDelay(SysCtlClockGet() / 3000);
}

void gpiob_int_handler(void)
{
	GPIOIntClear(GPIO_PORTB_BASE, GPIO_INT_PIN_1);
	ui8PrevCount++;
}

void gpiob_ini(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	//GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_1);

	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA,

			GPIO_PIN_TYPE_STD_WPD);



	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_1,
			GPIO_BOTH_EDGES);
	IntMasterEnable();                                              //全局中断使能
	IntEnable(INT_GPIOB);                                           //使能串口0中断
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_INT_PIN_1);          //使能串口0接收中断和接收超时中断

}

void UART0_init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	    GPIOPinConfigure(GPIO_PA0_U0RX);
	    GPIOPinConfigure(GPIO_PA1_U0TX);
	    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	    UARTStdioConfig(0, 115200, 16000000);

	    IntMasterEnable();                                              //全局中断使能
		IntEnable(INT_UART0);                                           //使能串口0中断
		UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);          //使能串口0接收中断和接收超时中断
		UARTEnable(UART0_BASE);                                        //使能串口1（UARTO）发送和接收

}

void timer0_int_handler(void)
{
	TimerIntClear(WTIMER0_BASE, TIMER_TIMB_TIMEOUT);
	ui8PrevCount++;
}

void timer0_ini(void)
{

	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
	TimerLoadSet(WTIMER0_BASE, TIMER_B, SysCtlClockGet());
	IntMasterEnable();
	TimerIntEnable(WTIMER0_BASE, TIMER_TIMB_TIMEOUT);
	IntEnable(INT_WTIMER0B);
//	TimerEnable(WTIMER0_BASE, TIMER_B);
}

void main (void)
{
	uint32_t time;

	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	timer0_ini();
	UART0_init();
	gpiob_ini();

	time = TimerValueGet(WTIMER0_BASE, TIMER_B);
	UARTprintf("%d ", time);
	TimerEnable(WTIMER0_BASE, TIMER_B);
	LCD12864_Delay(10);
	TimerDisable(WTIMER0_BASE, TIMER_B);
	time = TimerValueGet(WTIMER0_BASE, TIMER_B);
	UARTprintf("%d", time);

//	while(1);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	while(1)
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PrevCount);
//		while(1);
}
