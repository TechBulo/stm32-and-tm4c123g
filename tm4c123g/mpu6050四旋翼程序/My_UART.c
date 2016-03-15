/*
 * My_UART.c
 * descrirption: USART1 PB1&PB0 no interrupt
 *  Created on: 2014年7月28日
 *      Author: Itheaven
 */
#include "My_UART.h"
#include "My_uart.h"
#include "utils/uartstdio.h"

/*
void UART0_INIT(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	    GPIOPinConfigure(GPIO_PA0_U0RX);
	    GPIOPinConfigure(GPIO_PA1_U0TX);
	    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	    UARTStdioConfig(0, 115200, 16000000);
}
*/


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


/*void UART0_init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);                     //复用功能时钟
	GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);         //配置PAO、PA1为UART1 的 RXD0，TXD0，
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	   //配置PA0 PA1为串口引脚
	UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(), 115200,
						   (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
							 UART_CONFIG_PAR_NONE));
	IntMasterEnable();                                              //全局中断使能
	IntEnable(INT_UART0);                                           //使能串口0中断
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);          //使能串口0接收中断和接收超时中断
	UARTEnable(UART0_BASE);                                        //使能串口1（UARTO）发送和接收
}*/

void UART1_INIT(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);				//enble UART 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);                //enble GPIOB

    GPIOPinConfigure(GPIO_PB0_U1RX);						    //open GPIO alternate fuction
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1); //open

	UARTFIFOLevelSet(UART1_BASE,UART_FIFO_TX1_8,UART_FIFO_RX1_8);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));//

    UARTEnable(UART1_BASE);
}
void UART1_send_char(const char Byte)
{
    UARTCharPut(UART1_BASE, Byte);
}
char UART1_recive_char(void)
{
	char Byte;
	Byte = UARTCharGet(UART1_BASE);
	return Byte;
}
void UART1_send_string(const char *sendbuf)
{
	while(*sendbuf != '\0')
	{
		UART1_send_char(*sendbuf);
		sendbuf ++;
	}
}

void UART1_send_int64_t(const int64_t temp,char on)
{
	int64_t temp_p = temp;

	if(temp_p < 0)
	{
		UART1_send_char('-');
		temp_p = 0-temp_p;
	}
	if(temp_p > 0)
	{
		UART1_send_int64_t(temp_p/10,0);
		UART1_send_char(temp_p%10 + '0');
	}
	else if(temp_p == 0)
	{
		if(on)
			UART1_send_char('0');
	//	UART1_send_char('\n');
	}
}

void UART1_send_int32_t(const int32_t temp,char on)
{
	UART1_send_int64_t((int64_t) temp,on);
}

void UART1_send_int16_t(const int16_t temp,char on)
{
	UART1_send_int64_t((int64_t) temp,on);
}

void UART1_send_float(const float temp,int64_t lenth)
{
	int64_t i;
	float num = temp;
	int64_t temp_p;
   //发送整数部分
	if(num<0)
	{
		UART1_send_char('-');
		num = 0-num;
	}
	temp_p = (int64_t)num;
	UART1_send_int64_t(temp_p,1);
	UART1_send_char('.');
	//发送n位小数
	for(i=0; i<lenth; i++)
	{
		UART1_send_char(((int)(10*(num - temp_p)))%10 + '0');
		num = num*10;
		temp_p = (int64_t)num;
	}

}

void UART1_send_int64_t_string(const int64_t *temp, uint64_t num)
{
	uint64_t i;
	for(i=0; i<num; i++)
		UART1_send_int64_t(temp[i],1);
}
void UART1_send_int32_t_string(const int32_t *temp, uint64_t num)
{
	uint64_t i;
	for(i=0; i<num; i++)
		UART1_send_int32_t(temp[i],1);
}
void UART1_send_int16_t_string(const int16_t *temp, uint64_t num)
{
	uint64_t i;
	for(i=0; i<num; i++)
		UART1_send_int16_t(temp[i],1);
}
void UART1_send_char_string(const unsigned char *temp, uint64_t num)
{
	uint64_t i;
	for(i=0; i<num; i++)
			UART1_send_char(temp[i]);
}
void UART1_send_float_string(const float *temp, int64_t lenth, uint64_t num)
{
	uint64_t i;
	for(i=0; i<num; i++)
		UART1_send_float(temp[i],lenth);
}

void UART1_recive_string(char *reciveBuf,int num)
{
	int i;
	for(i=0; i<num;i++)
		reciveBuf[i] = UART1_recive_char();
}

void UART0_send_char(const char Byte)
{
    UARTCharPut(UART0_BASE, Byte);
}

void UART0_send_string(const char *sendbuf)
{
	while(*sendbuf != '\0')
	{
		UART0_send_char(*sendbuf);
		sendbuf ++;
	}
}
