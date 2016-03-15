/*
 * Four_PWM.c
 *
 *  Created on: 2014年10月14日
 *      Author: hanken
 */
#include "Four_PWM.h"
uint32_t PWM_0_Period;
uint32_t PWM_1_Period;
uint32_t PWM_2_Period;
uint32_t PWM_3_Period;
float PWM_0_prop = 0.7;
float PWM_1_prop = 0.7;
float PWM_2_prop = 0.7;
float PWM_3_prop = 0.7;
uint32_t PWM_0_Width = 8000 * 0.6;//这是油门加到0.7时候的Width参数
uint32_t PWM_1_Width = 8000 * 0.6;
uint32_t PWM_2_Width = 8000 * 0.6;
uint32_t PWM_3_Width = 8000 * 0.6;
//红灯逆时针0123PWM控制
void InitFourPWM()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);//使能PWM0模块
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能PWM0输出所在GPIO
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);     // PWM时钟配置：不分频
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);

	GPIOPinConfigure(GPIO_PB7_M0PWM1);    //#define GPIO_PB6_M0PWM0         0x00011804
	GPIOPinConfigure(GPIO_PB6_M0PWM0);    //#define GPIO_PB7_M0PWM1         0x00011C04
	GPIOPinConfigure(GPIO_PB4_M0PWM2);    //#define GPIO_PB4_M0PWM2         0x00011804
	GPIOPinConfigure(GPIO_PB5_M0PWM3);    //#define GPIO_PB5_M0PWM3         0x00011C04


    //配置PWM发生器0：加减计数，不同步
    PWMGenConfigure(PWM0_BASE,PWM_GEN_0,PWM_GEN_MODE_UP_DOWN| PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM0_BASE,PWM_GEN_1,PWM_GEN_MODE_UP_DOWN| PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM0_BASE,PWM_GEN_2,PWM_GEN_MODE_UP_DOWN| PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM0_BASE,PWM_GEN_3,PWM_GEN_MODE_UP_DOWN| PWM_GEN_MODE_NO_SYNC);


}

void SetPWM(void)
{
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 8000);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 8000);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 8000);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 8000);


	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWM_0_Width);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWM_1_Width);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, PWM_2_Width);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, PWM_3_Width);
	//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 8000*0.60);
	//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 8000*0.60);
	//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 8000*0.60);
	//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 8000*0.60);
}
void EnablePWM(void)
{
	PWMOutputState(PWM0_BASE, (PWM_OUT_0_BIT |PWM_OUT_1_BIT|PWM_OUT_2_BIT|PWM_OUT_3_BIT), true);
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);
	PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}

void DisEnablePWM(void)
{
	PWMOutputState(PWM0_BASE, (PWM_OUT_0_BIT |PWM_OUT_1_BIT|PWM_OUT_2_BIT|PWM_OUT_3_BIT), false);
	PWMGenDisable(PWM0_BASE, PWM_GEN_0);
	PWMGenDisable(PWM0_BASE, PWM_GEN_1);
	PWMGenDisable(PWM0_BASE, PWM_GEN_2);
	PWMGenDisable(PWM0_BASE, PWM_GEN_3);
}
