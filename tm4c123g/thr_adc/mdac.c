#ifndef TARGET_IS_BLIZZARD_RA1
#define TARGET_IS_BLIZZARD_RA1
#endif

#ifndef PART_TM4C123GH6PM
#define PART_TM4C123GH6PM
#endif
//----------------------------------------------------------------------------
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/ssi.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
//----------------------------------------------------------------------------

#define DAC_A  0x4000 	//DAC A
#define DAC_B  0x8000	//DAC B
#define DAC_AB 0xC000	//DAC A and B


void delay()
{
	unsigned int counter = 0;
	while(counter > 1000){
		counter++;
	}
}

//-------------------------------------------------------------------------------------------
// @brief SSIģ��ʹ�ܣ�����������ض˿ڳ�ʼ״̬
// @param none
// @return none
//		 _______________
//				        |
//			PF2(SSI1Clk)|-->SPICLK	ʱ���źŶ�
//	TIVA	PF3(SSI1Fss)|-->SYNC	֡�źŶ�
//			PF1(SSI1Tx) |-->SDIN	SSI���ݷ��Ͷˣ�LM4F120->DAC8802��
//			PB2(GPIO)	|-->LDAC
//			PB3(GPIO)	|-->RS
//			PC4(GPIO)	|-->MSB
//		________________|
//
//-------------------------------------------------------------------------------------------
void ssi_en()
{

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);			//ʹ������SSI1ģ��
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);		//ʹ��SSI1ʹ�õ�����GPIOF

    //SSI1�˿ڹ���ʹ��
    ROM_GPIOPinConfigure(GPIO_PF2_SSI1CLK);					//PF2���ù�������ΪSSI1CLK��ʱ����
    ROM_GPIOPinConfigure(GPIO_PF3_SSI1FSS);					//PF3���ù�������ΪSSI1FSS��Ƭѡ��
    ROM_GPIOPinConfigure(GPIO_PF1_SSI1TX);					//PF1���ù�������ΪSSI1TX�����ݷ�����

	//----------------------------------------------------------------------------
	//LDAC�ø�
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);
	ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
	//RS�ø�
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
	ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
	//MSB�ø�
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
	ROM_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4);
	//----------------------------------------------------------------------------

    ROM_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);	//����PF1��PF2��PF3������SSI1ʹ��

    ROM_SSIConfigSetExpClk(SSI1_BASE, ROM_SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 16);	//�˿�ģʽ:1M,16λ����

    ROM_SSIEnable(SSI1_BASE);				//ʹ��SSI
}

//----------------------------------------------------------------------------
//  @brief		��dac8802��������
//	@param		unsigned long val��ȡֵ��Χ0~16384
//	@return		0�� ��������ȷ��
//				1������ɹ���
//----------------------------------------------------------------------------
unsigned char ssi_send_2_dac8802(unsigned long val)
{
	if(val > 16384) return 0;
    ROM_SSIDataPut(SSI1_BASE, DAC_AB + val);	//������+
    while(ROM_SSIBusy(SSI1_BASE));				//�ȴ��������

    //���ݷ��ͽ���ʱ��LDAC����Ҫһ����ƽ�����䣨H->L->H��
	ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);
	delay();
	ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
	delay();
    return 1;
}
