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
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"

#define BUF 			10		//保存最大值的缓冲区大小
#define SAM_FREQ		2000		//采样频率
#define SAM_CNT			100		//每组采样数量
#define SAM_GRP			5		//采样组数   <= BUF

#define SAM_CHG_MAX		3		//是否变化标准

typedef struct{
	int goaway[3]	;//保存最近的3个数值
	int count 		;//已由元素下标
	int avg			;//平均值
}cnt_sv;//保存数据

int pui32ADC0Value 	;
int val[BUF] = {0} 	;
int lastres = 0 	;
int chgfg   = 0		; //是否变化

static char *itoa(int value, char *string, int radix);
void uartinit(void);
void uartputs(char *str);
char *uartputi(char *str, int num);
void adcinit(void);
//采样值	 采样频率	 采样点数
int  getval(int num, int freq, int cnt);
int  getmax(int *tmp);
int  getavg(int *tmp);
int  refreshdata(cnt_sv *data);

int  refreshdata(cnt_sv *data)
{
	int rtn  = 0;
	int up   = 0;
    int down = 0;

	if(3 == data->count){
		data->count = 0;
		up      = data->goaway[2] + data->goaway[2] * 0.1;
		down    = data->goaway[2] - data->goaway[2] * 0.1;
		if(up >= data->goaway[1] && down <= data->goaway[1] && up >= data->goaway[0] && down <= data->goaway[0]){
			rtn = 1;
			data->avg = (data->goaway[0] + data->goaway[1] + data->goaway[2])/3;
		}
	}

	return rtn;
}

int  getavg(int *tmp)
{
	int sum = 0;
	int avg = 0;
	int i   = 0;

	while(i < SAM_GRP){
		sum += tmp[i++];
	}

	avg = sum/(SAM_GRP);

	return avg ;
}

int  getmax(int *tmp)
{
	int res  = tmp[0];
	int i    = 0;

	while(i < SAM_CNT){
		if(tmp[i++] > res){
			res = tmp[i-1];
		}
	}

	return res ;
}


int  getval(int num, int freq, int cnt)
{
	int res = 0  ;
	int tmp[SAM_CNT] = 0  ;
	int t = 0;
	int i   = 0  ;
	int j   = 0  ;

	if(cnt > SAM_CNT){
		cnt = SAM_CNT ;
	}

	while(SAM_GRP > j){
		while(i < cnt){
			SysCtlDelay(SysCtlClockGet()/freq);
			ADCProcessorTrigger(ADC0_BASE, 3);
			ADCSequenceDataGet(ADC0_BASE, 3, &pui32ADC0Value);
			//if(tmp[i-1] < (t = pui32ADC0Value*3300 / 4096))
			t = pui32ADC0Value;
			tmp[i++] = t;
		}
		i = 0;
		val[j++] = getmax(tmp);
		memset(tmp, 0x00, sizeof(char) * SAM_CNT);
	}

	res = getavg(val);

	return res;
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

void adcinit(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);
}

void main(void)
{

	char    buf[10] = {0}  ;
	cnt_sv  tmp     = {{0,0,0}, 0, 0};


	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	uartinit();
	adcinit();
	uartputs("hello\r\n");

 	lastres = tmp.goaway[tmp.count++] = getval(pui32ADC0Value, SAM_FREQ, SAM_CNT);

	uartputs(uartputi(buf, lastres));
	uartputs(" mV\r\n");
	memset(buf, 0x00, sizeof(char)*10);

	while(1)
	{
		//uartputs(uartputi(buf, pui32ADC0Value));
		tmp.goaway[tmp.count++] = getval(pui32ADC0Value, SAM_FREQ, SAM_CNT);
		if(1 == refreshdata(&tmp)){
			lastres = (tmp.avg)*33000 / 4096;
		}

		uartputs(uartputi(buf, lastres/10));
		uartputs(" mV\r\n");
		memset(buf, 0x00, sizeof(char)*10);

	}
}
