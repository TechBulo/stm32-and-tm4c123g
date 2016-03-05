/**************************************************************************
* Description        : ����������STM32���룬����HC-HR04ģ�飬��ģ��Trig�ź�Ĭ�ϸߵ�ƽ����Ҫ��10uS���ϵĵ͵�ƽ����
*                      ��ЩHC-HR04ģ����Ҫ�ߵ�ƽ��������������޸ļ���ʹ��
*                      GPIOB_12��Ϊ�����ܽţ�GPIOA_12��Ϊ���չܽ�
*                      ÿ��һ�봮��1��ӡһ�ξ��볤��
***************************************************************************/
#include "stm32f10x.h"

void Sonic_Config(void);
static void Sonic_EXTI_Configuration(void);
static void Sonic_GPIO_Configuration(void);
static void Sonic_Delay_1ms(u32 time);
static void Timer_Configuration(void);
void Sonic_Start(void);
static void NVIC_Configuration(void);
