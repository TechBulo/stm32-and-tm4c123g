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
// @brief SSI模块使能，并且设置相关端口初始状态
// @param none
// @return none
//		 _______________
//				        |
//			PF2(SSI1Clk)|-->SPICLK	时钟信号端
//	TIVA	PF3(SSI1Fss)|-->SYNC	帧信号端
//			PF1(SSI1Tx) |-->SDIN	SSI数据发送端（LM4F120->DAC8802）
//			PB2(GPIO)	|-->LDAC
//			PB3(GPIO)	|-->RS
//			PC4(GPIO)	|-->MSB
//		________________|
//
//-------------------------------------------------------------------------------------------
void ssi_en()
{

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);			//使能外设SSI1模块
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);		//使能SSI1使用的外设GPIOF

    //SSI1端口功能使能
    ROM_GPIOPinConfigure(GPIO_PF2_SSI1CLK);					//PF2复用功能配置为SSI1CLK，时钟线
    ROM_GPIOPinConfigure(GPIO_PF3_SSI1FSS);					//PF3复用功能配置为SSI1FSS，片选线
    ROM_GPIOPinConfigure(GPIO_PF1_SSI1TX);					//PF1复用功能配置为SSI1TX，数据发送线

	//----------------------------------------------------------------------------
	//LDAC置高
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);
	ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
	//RS置高
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
	ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
	//MSB置高
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
	ROM_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4);
	//----------------------------------------------------------------------------

    ROM_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);	//配置PF1，PF2，PF3供外设SSI1使用

    ROM_SSIConfigSetExpClk(SSI1_BASE, ROM_SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 16);	//端口模式:1M,16位数据

    ROM_SSIEnable(SSI1_BASE);				//使能SSI
}

//----------------------------------------------------------------------------
//  @brief		向dac8802发送数据
//	@param		unsigned long val，取值范围0~16384
//	@return		0， 参数不正确；
//				1，传输成功；
//----------------------------------------------------------------------------
unsigned char ssi_send_2_dac8802(unsigned long val)
{
	if(val > 16384) return 0;
    ROM_SSIDataPut(SSI1_BASE, DAC_AB + val);	//发数据+
    while(ROM_SSIBusy(SSI1_BASE));				//等待发送完成

    //数据发送结束时，LDAC线需要一个电平的跳变（H->L->H）
	ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);
	delay();
	ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
	delay();
    return 1;
}
