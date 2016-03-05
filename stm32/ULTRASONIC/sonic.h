/**************************************************************************
* Description        : 超声波测距的STM32代码，采用HC-HR04模块，该模块Trig信号默认高电平，需要给10uS以上的低电平触发
*                      有些HC-HR04模块需要高电平触发，将程序简单修改即可使用
*                      GPIOB_12作为触发管脚，GPIOA_12作为接收管脚
*                      每隔一秒串口1打印一次距离长度
***************************************************************************/
#include "stm32f10x.h"

void Sonic_Config(void);
static void Sonic_EXTI_Configuration(void);
static void Sonic_GPIO_Configuration(void);
static void Sonic_Delay_1ms(u32 time);
static void Timer_Configuration(void);
void Sonic_Start(void);
static void NVIC_Configuration(void);
