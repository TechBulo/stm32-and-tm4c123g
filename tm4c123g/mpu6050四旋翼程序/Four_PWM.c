/*
 * Four_PWM.c
 *
 *  Created on: 2014��10��14��
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
uint32_t PWM_0_Width = 8000 * 0.6;//�������żӵ�0.7ʱ���Width����
uint32_t PWM_1_Width = 8000 * 0.6;
uint32_t PWM_2_Width = 8000 * 0.6;
uint32_t PWM_3_Width = 8000 * 0.6;
//�����ʱ��0123PWM����
void InitFourPWM()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);//ʹ��PWM0ģ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ��PWM0�������GPIO
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);     // PWMʱ�����ã�����Ƶ
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);

	GPIOPinConfigure(GPIO_PB7_M0PWM1);    //#define GPIO_PB6_M0PWM0         0x00011804
	GPIOPinConfigure(GPIO_PB6_M0PWM0);    //#define GPIO_PB7_M0PWM1         0x00011C04
	GPIOPinConfigure(GPIO_PB4_M0PWM2);    //#define GPIO_PB4_M0PWM2         0x00011804
	GPIOPinConfigure(GPIO_PB5_M0PWM3);    //#define GPIO_PB5_M0PWM3         0x00011C04


    //����PWM������0���Ӽ���������ͬ��
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
