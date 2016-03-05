#ifndef TARGET_IS_BLIZZARD_RA1
#define TARGET_IS_BLIZZARD_RA1
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "LCDDriver.h"
//#include "LCD_Matrix.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "mdac.h"


/************************************************************************
 * 初始化ADC获取GPIO_ADC的采样值
 * 		————|
 *				|
 *		M4	 PD2|<--ADC1（GPIO_ADC）		模数转换信号源
 *		________|
 *
 ***********************************************************************/
//#define ADC_BASE		ADC0_BASE			// 使用ADC0
#define SequenceNum1 	1					// 使用序列1
void Init_ADC()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);			//使能ADC0外设

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);		//使能外设GPIOD

	ROM_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);		//PD2供外设ADC0使用

	ROM_ADCSequenceConfigure(ADC_BASE, SequenceNum1, ADC_TRIGGER_PROCESSOR, 0);	//采样序列配置，使用序列1，处理器触发采样模式

	ROM_ADCSequenceStepConfigure(ADC_BASE, SequenceNum1, 0, ADC_CTL_CH5 | ADC_CTL_IE |	//采样序列Step配置，使用Step 0，CH5采样
							 ADC_CTL_END);												//(CH5<-->PD2),采样结束并且产生一个中断信号

	ROM_ADCSequenceEnable(ADC_BASE, SequenceNum1);			//使能采样序列

	ROM_ADCIntClear(ADC_BASE, SequenceNum1);				//清除中断标志
}

//-------------------
unsigned char index_buff = 0;					//sample_buff的下标值
unsigned char index_ver = 0;					//sample_ver的下标值
unsigned int sample_ver[3] = {0};				//保留连续的3个采样值
unsigned int sample_buff[10] = {0};			//保存1sample_ver中计算得出的10个最大值
int average_max_sample = 0;						//得出total_sample的平均值作为平均最大值（峰值）

int main(void)
{
	unsigned int dac8802_val = 4000;
	uint32_t pui32ADCWheel_Value;			// 保存ADC采样值
	uint32_t pui32ADCPD2_Value;			// 保存ADC采样值
	unsigned int pre_adc = 0;
	unsigned int cur_adc = 0;
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); 	//系统时钟配置

	LCD_Enable();						// 使能LCD
	LCD_Init();							// 初始化LCD
	LCD_ScreenClr();					// 清屏

	Init_ADCWheel();					//初始化ADCWheel模块
	Init_ADC();

	ssi_en();							//初始化SSI1通信
	//LCD默认显示
	LCD_Draw_Char('P', LINE_ONE, CHAR_WIDTH * 0);
	LCD_Draw_Char('e', LINE_ONE, CHAR_WIDTH * 1);
	LCD_Draw_Char('a', LINE_ONE, CHAR_WIDTH * 2);
	LCD_Draw_Char('k', LINE_ONE, CHAR_WIDTH * 3);

	LCD_Draw_Char('d', LINE_ONE, CHAR_WIDTH * 5);
	LCD_Draw_Char('e', LINE_ONE, CHAR_WIDTH * 6);
	LCD_Draw_Char('t', LINE_ONE, CHAR_WIDTH * 7);
	LCD_Draw_Char('e', LINE_ONE, CHAR_WIDTH * 8);
	LCD_Draw_Char('c', LINE_ONE, CHAR_WIDTH * 9);
	LCD_Draw_Char('t', LINE_ONE, CHAR_WIDTH * 10);
	LCD_Draw_Char('i', LINE_ONE, CHAR_WIDTH * 11);
	LCD_Draw_Char('o', LINE_ONE, CHAR_WIDTH * 12);
	LCD_Draw_Char('n', LINE_ONE, CHAR_WIDTH * 13);

	LCD_Draw_Char('v', LINE_TWO, CHAR_WIDTH * 0);
	LCD_Draw_Char('o', LINE_TWO, CHAR_WIDTH * 1);
	LCD_Draw_Char('l', LINE_TWO, CHAR_WIDTH * 2);
	LCD_Draw_Char('t', LINE_TWO, CHAR_WIDTH * 3);
	LCD_Draw_Char('a', LINE_TWO, CHAR_WIDTH * 4);
	LCD_Draw_Char('g', LINE_TWO, CHAR_WIDTH * 5);
	LCD_Draw_Char('e', LINE_TWO, CHAR_WIDTH * 6);
	LCD_Draw_Char(':', LINE_TWO, CHAR_WIDTH * 7);


	LCD_Draw_Char('V', LINE_TWO, 127-8);

	while(1)
	{
		ADCWheel_ValueGet(&pui32ADCWheel_Value);
		cur_adc = pui32ADCWheel_Value;

		//两次采样值相差60时更新dac8802参数（已步进60更新dac8802）
		if(cur_adc-pre_adc > 60 || pre_adc-cur_adc > 60)
		{
			pre_adc = cur_adc;										//保存当前采样值

			dac8802_val = (16384 / 4096) * pui32ADCWheel_Value;		//根据采样值计算dac8802参数，dac8802内部为14位的DA，而M4
																	//进行ADCWheel（滚轮）采样的AD为12位，此处进行比例放大
			int i;
			unsigned char data[5] = {0};
			data[0] = dac8802_val / 10000;							//根据LCD显示拆分数据
			data[1] = (dac8802_val / 1000) % 10;
			data[2] = (dac8802_val / 100) % 10;
			data[3] = (dac8802_val / 10) % 10;
			data[4] = (dac8802_val / 1) % 10;

			for(i = 0; i < 5; ++i)
			{
				LCD_Draw_Char('0' + data[i], LINE_FOUR, 40 + 8 * i);
			}
		}

		ssi_send_2_dac8802(dac8802_val);							//将dac8802参数值通过SSI传输至dac8802

		//---------------------------------------------------------------------------------------------------------------
		//PD2采集，并进行峰峰值检测
		ADCProcessorTrigger(ADC_BASE, SequenceNum1);				//处理器触发采样
		while(!ADCIntStatus(ADC_BASE, SequenceNum1, false))		//等待采样结束
		{
		}
		ADCIntClear(ADC_BASE, SequenceNum1);						//清除中断标志
		ADCSequenceDataGet(ADC_BASE, SequenceNum1, &pui32ADCPD2_Value);	//读取采样值

		unsigned long cur_sample = pui32ADCPD2_Value * 3300 / 4096;	//计算得出真实值，单位mV

		//-----------------------------------------------------------------------------------------------------------
		//以下为简单峰值检测程序部分，
		sample_ver[index_ver++] = cur_sample;			//保存当前采样值

		if(index_ver > 2)
			index_ver = 0;

		//当采样值达到三个时，进行最大值比较，当中间值最大时认为该点为最大值（局部峰值）
		if((sample_ver[0] < sample_ver[1])&&(sample_ver[1] > sample_ver[2]))
		{
			sample_buff[index_buff] = sample_ver[1];			//将得到最大值存放到sample_buff中

			index_buff = index_buff + 1;
		}

		//当sample_buff中的数据达到10个时，取平均值作为峰值
		if(index_buff >= 10)
		{

			uint8_t index;
			uint32_t average = 0;
			for(index = 0;index<10;index++)
			{
				average +=sample_buff[index] ;
			}
			average_max_sample = average / 10;
			index_buff = 0;
		}
			//-----------------------------------------------------------------------------------------------
			//进行LCD显示，数据拆分
			unsigned char max_data[4] = {0};
			max_data[0] = average_max_sample / 1000;
			max_data[1] = (average_max_sample / 100) % 10;
			max_data[2] = (average_max_sample / 10) % 10;
			max_data[3] = (average_max_sample / 1) % 10;

			uint8_t i;
			for(i = 0; i < 4; ++i)
			{
				LCD_Draw_Char('.', LINE_TWO, 70 + 8 * 1);
				if(i > 0){
					LCD_Draw_Char('0' + max_data[i], LINE_TWO, 70 + 8 * (i + 1));
				}else{
					LCD_Draw_Char('0' + max_data[i], LINE_TWO, 70 + 8 * i);
				}
			}
			//------------------------------------------------------------------------------------------
			ROM_SysCtlDelay(SysCtlClockGet() / 12);		//延时250 ms

	}
}
//---------------------------------------------------------------------------
