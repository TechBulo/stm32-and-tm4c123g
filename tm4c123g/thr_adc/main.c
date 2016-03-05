/*
 进行一路ADC采样
 */

#define PART_TM4C123GH6PM
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_adc.h"
#include "inc/hw_ints.h"
#include "driverlib/fpu.h"
#include "grlib/grlib.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "LCD12864.h"
#include "driverlib/rom.h"
#include "mdac.h"

#define ad9850_periph			(SYSCTL_PERIPH_GPIOA)
#define ad9850_gpio				(GPIO_PORTA_BASE)
#define ad9850_w_clk			(GPIO_PIN_2)
#define ad9850_fq_up			(GPIO_PIN_3)
#define ad9850_rest				(GPIO_PIN_4)
#define ad9850_bit_data			(GPIO_PIN_5)

#define GPIO_ResetBits(x, y)	(GPIOPinWrite(x, y, ~y))
#define GPIO_SetBits(x, y)		(GPIOPinWrite(x, y, y))

#define STEPV			100.0	//步进（mV）
#define STEPF			100.0	//步进（hz）
#define SQUWV			1000.0	//方波峰值
#define SQUMV(MV)	 	( 16384.0 / (SQUWV/MV) )

#define MAXFREQ			10000 //最高频率 10K
static char *itoa(int value, char *string, int radix);
void uartinit(void);
void uartputs(char *str);
char *uartputi(char *str, int num);

void ad9850_reset()
{
	GPIO_ResetBits(ad9850_gpio, ad9850_w_clk);
	GPIO_ResetBits(ad9850_gpio, ad9850_fq_up);
//rest信号
	GPIO_ResetBits(ad9850_gpio, ad9850_rest);
	GPIO_SetBits(ad9850_gpio, ad9850_rest);
	GPIO_ResetBits(ad9850_gpio, ad9850_rest);
}

void ad9850_reset_serial()
{
	GPIO_ResetBits(ad9850_gpio, ad9850_w_clk);
	GPIO_ResetBits(ad9850_gpio, ad9850_fq_up);
//rest信号
	GPIO_ResetBits(ad9850_gpio, ad9850_rest);
	GPIO_SetBits(ad9850_gpio, ad9850_rest);
	GPIO_ResetBits(ad9850_gpio, ad9850_rest);
//w_clk信号
	GPIO_ResetBits(ad9850_gpio, ad9850_w_clk);
	GPIO_SetBits(ad9850_gpio, ad9850_w_clk);
	GPIO_ResetBits(ad9850_gpio, ad9850_w_clk);
//fq_up信号
	GPIO_ResetBits(ad9850_gpio, ad9850_fq_up);
	GPIO_SetBits(ad9850_gpio, ad9850_fq_up);
	GPIO_ResetBits(ad9850_gpio, ad9850_fq_up);
}

void ad9850_wr_serial(unsigned char w0,double frequence)
{
	unsigned char i,w;
	long int y;
	double x;
	//计算频率的HEX值
	x=4294967295/125;//适合125M晶振
	//如果时钟频率不为180MHZ，修改该处的频率值，单位MHz  ！！！
	frequence=frequence/1000000;
	frequence=frequence*x;
	y=frequence;
	//写w4数据
	w=(y>>=0);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			GPIO_SetBits(ad9850_gpio, ad9850_bit_data);
		else
			GPIO_ResetBits(ad9850_gpio, ad9850_bit_data);
		GPIO_SetBits(ad9850_gpio, ad9850_w_clk);
		GPIO_ResetBits(ad9850_gpio, ad9850_w_clk);
	}
	//写w3数据
	w=(y>>8);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			GPIO_SetBits(ad9850_gpio, ad9850_bit_data);
		else
			GPIO_ResetBits(ad9850_gpio, ad9850_bit_data);
		GPIO_SetBits(ad9850_gpio, ad9850_w_clk);
		GPIO_ResetBits(ad9850_gpio, ad9850_w_clk);
	}
	//写w2数据
	w=(y>>16);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			GPIO_SetBits(ad9850_gpio, ad9850_bit_data);
		else
			GPIO_ResetBits(ad9850_gpio, ad9850_bit_data);
		GPIO_SetBits(ad9850_gpio, ad9850_w_clk);
		GPIO_ResetBits(ad9850_gpio, ad9850_w_clk);
	}
	//写w1数据
	w=(y>>24);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			GPIO_SetBits(ad9850_gpio, ad9850_bit_data);
		else
			GPIO_ResetBits(ad9850_gpio, ad9850_bit_data);
		GPIO_SetBits(ad9850_gpio, ad9850_w_clk);
		GPIO_ResetBits(ad9850_gpio, ad9850_w_clk);
	}
	//写w0数据
	w=w0;
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			GPIO_SetBits(ad9850_gpio, ad9850_bit_data);
		else
			GPIO_ResetBits(ad9850_gpio, ad9850_bit_data);
		GPIO_SetBits(ad9850_gpio, ad9850_w_clk);
		GPIO_ResetBits(ad9850_gpio, ad9850_w_clk);
	}
	//移入始能
	GPIO_SetBits(ad9850_gpio, ad9850_fq_up);
	GPIO_ResetBits(ad9850_gpio, ad9850_fq_up);
}


void ad9850init()
{
	SysCtlPeripheralEnable(ad9850_periph);
	GPIOPinTypeGPIOOutput(ad9850_gpio, ad9850_w_clk | ad9850_fq_up | ad9850_rest | ad9850_bit_data);
}
/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -string 转换后的字符串
 *
 * 返回  ：字符串 string
 *
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;
    return string;
}


void uartinit(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(0x00000001);
	GPIOPinConfigure(0x00000001);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
	                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	                         UART_CONFIG_PAR_NONE));
	UARTFIFOEnable(UART0_BASE);
	UARTFIFOLevelSet(UART0_BASE,
			UART_FIFO_TX1_8,
			UART_FIFO_TX1_8);
}

void uartputs(char *str)
{
	int i = 0;

	while('\0' != str[i]){
		UARTCharPut(UART0_BASE, str[i++]);
	}
}

char *uartputi(char *str, int num)
{
	return itoa(num, str, 10);
}

void main(void)
{
	int stepv = STEPV ;
	int stepf = STEPF ;
	int origv = 500  ;
	int origf = 1000   ;
	char buf[20] = {0};
	int mul  = 8192 ;//SQUMV(origv);
	char btn = 0;

	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	uartinit();
	uartputs("hello\r\n");

	KEY_GPIO_Config();

	LCD12864Init();
	LCD12864Cls();
	LCD12864WRStr(1,1,"hello");

	ssi_en();
	ssi_send_2_dac8802(mul);//将dac8802参数值通过SSI传输至dac8802

	ad9850init();
	ad9850_reset_serial();
	ad9850_wr_serial(0x00,2000);

	//mul = SQUMV(900);
	//ssi_send_2_dac8802(mul);

	while(1){

		if(0  != (btn = ENTER_Key())){

			LCD12864Cls();
			if('3' == btn)
			{
				if(origv < SQUWV)
					origv += stepv;
			}else if('A' == btn)
			{
				if(origv > 0)
					origv -= stepv;
			}else if('B' == btn){
				if(origf < MAXFREQ)
					origf += stepf;
			}else if('6' == btn){
				if(origf > 100)
					origf -= stepf;
			}
			buf[0] = btn;
			buf[1] = 0;
			LCD12864WRStr(2,2,buf);

		}

		mul = SQUMV(origv);
		ssi_send_2_dac8802(mul);
		ad9850_wr_serial(0x00,origf);
		SysCtlDelay(20000);

	}
}
