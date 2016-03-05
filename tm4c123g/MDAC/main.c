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
 * ��ʼ��ADC��ȡGPIO_ADC�Ĳ���ֵ
 * 		��������|
 *				|
 *		M4	 PD2|<--ADC1��GPIO_ADC��		ģ��ת���ź�Դ
 *		________|
 *
 ***********************************************************************/
//#define ADC_BASE		ADC0_BASE			// ʹ��ADC0
#define SequenceNum1 	1					// ʹ������1
void Init_ADC()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);			//ʹ��ADC0����

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);		//ʹ������GPIOD

	ROM_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);		//PD2������ADC0ʹ��

	ROM_ADCSequenceConfigure(ADC_BASE, SequenceNum1, ADC_TRIGGER_PROCESSOR, 0);	//�����������ã�ʹ������1����������������ģʽ

	ROM_ADCSequenceStepConfigure(ADC_BASE, SequenceNum1, 0, ADC_CTL_CH5 | ADC_CTL_IE |	//��������Step���ã�ʹ��Step 0��CH5����
							 ADC_CTL_END);												//(CH5<-->PD2),�����������Ҳ���һ���ж��ź�

	ROM_ADCSequenceEnable(ADC_BASE, SequenceNum1);			//ʹ�ܲ�������

	ROM_ADCIntClear(ADC_BASE, SequenceNum1);				//����жϱ�־
}

//-------------------
unsigned char index_buff = 0;					//sample_buff���±�ֵ
unsigned char index_ver = 0;					//sample_ver���±�ֵ
unsigned int sample_ver[3] = {0};				//����������3������ֵ
unsigned int sample_buff[10] = {0};			//����1sample_ver�м���ó���10�����ֵ
int average_max_sample = 0;						//�ó�total_sample��ƽ��ֵ��Ϊƽ�����ֵ����ֵ��

int main(void)
{
	unsigned int dac8802_val = 4000;
	uint32_t pui32ADCWheel_Value;			// ����ADC����ֵ
	uint32_t pui32ADCPD2_Value;			// ����ADC����ֵ
	unsigned int pre_adc = 0;
	unsigned int cur_adc = 0;
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); 	//ϵͳʱ������

	LCD_Enable();						// ʹ��LCD
	LCD_Init();							// ��ʼ��LCD
	LCD_ScreenClr();					// ����

	Init_ADCWheel();					//��ʼ��ADCWheelģ��
	Init_ADC();

	ssi_en();							//��ʼ��SSI1ͨ��
	//LCDĬ����ʾ
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

		//���β���ֵ���60ʱ����dac8802�������Ѳ���60����dac8802��
		if(cur_adc-pre_adc > 60 || pre_adc-cur_adc > 60)
		{
			pre_adc = cur_adc;										//���浱ǰ����ֵ

			dac8802_val = (16384 / 4096) * pui32ADCWheel_Value;		//���ݲ���ֵ����dac8802������dac8802�ڲ�Ϊ14λ��DA����M4
																	//����ADCWheel�����֣�������ADΪ12λ���˴����б����Ŵ�
			int i;
			unsigned char data[5] = {0};
			data[0] = dac8802_val / 10000;							//����LCD��ʾ�������
			data[1] = (dac8802_val / 1000) % 10;
			data[2] = (dac8802_val / 100) % 10;
			data[3] = (dac8802_val / 10) % 10;
			data[4] = (dac8802_val / 1) % 10;

			for(i = 0; i < 5; ++i)
			{
				LCD_Draw_Char('0' + data[i], LINE_FOUR, 40 + 8 * i);
			}
		}

		ssi_send_2_dac8802(dac8802_val);							//��dac8802����ֵͨ��SSI������dac8802

		//---------------------------------------------------------------------------------------------------------------
		//PD2�ɼ��������з��ֵ���
		ADCProcessorTrigger(ADC_BASE, SequenceNum1);				//��������������
		while(!ADCIntStatus(ADC_BASE, SequenceNum1, false))		//�ȴ���������
		{
		}
		ADCIntClear(ADC_BASE, SequenceNum1);						//����жϱ�־
		ADCSequenceDataGet(ADC_BASE, SequenceNum1, &pui32ADCPD2_Value);	//��ȡ����ֵ

		unsigned long cur_sample = pui32ADCPD2_Value * 3300 / 4096;	//����ó���ʵֵ����λmV

		//-----------------------------------------------------------------------------------------------------------
		//����Ϊ�򵥷�ֵ�����򲿷֣�
		sample_ver[index_ver++] = cur_sample;			//���浱ǰ����ֵ

		if(index_ver > 2)
			index_ver = 0;

		//������ֵ�ﵽ����ʱ���������ֵ�Ƚϣ����м�ֵ���ʱ��Ϊ�õ�Ϊ���ֵ���ֲ���ֵ��
		if((sample_ver[0] < sample_ver[1])&&(sample_ver[1] > sample_ver[2]))
		{
			sample_buff[index_buff] = sample_ver[1];			//���õ����ֵ��ŵ�sample_buff��

			index_buff = index_buff + 1;
		}

		//��sample_buff�е����ݴﵽ10��ʱ��ȡƽ��ֵ��Ϊ��ֵ
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
			//����LCD��ʾ�����ݲ��
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
			ROM_SysCtlDelay(SysCtlClockGet() / 12);		//��ʱ250 ms

	}
}
//---------------------------------------------------------------------------
