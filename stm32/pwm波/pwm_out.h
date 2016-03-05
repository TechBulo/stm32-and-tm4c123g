/******************************************************************************
*具体注意事项参考readme.txt
*
******************************************************************************/

#ifndef _PWM_OUT_H_
#define _PWM_OUT_H_

#include <stm32f10x.h>

/*
实验中     电机一     D10.......EN+  //控制使
                      D11.......DIR+方向高电平为正
                      A6.......CP-脉冲(a6不能改变)
                      
           电机二     B6.......CP-脉冲
                      D8.......EN+
                      D9.......DIR+
*/
/*******************代码移植区*****************************/
#define PWM1_TIM TIM3
#define RCC_APB1Periph_PWM1_TIM RCC_APB1Periph_TIM3

#define PWM2_TIM TIM4
#define RCC_APB1Periph_PWM2_TIM RCC_APB1Periph_TIM4

#define RCC_APB2Periph_PWM1_GPIO_0	(RCC_APB2Periph_GPIOA)
#define RCC_APB2Periph_PWM1_GPIO_1	(RCC_APB2Periph_GPIOD)

#define RCC_APB2Periph_PWM2_GPIO_0	(RCC_APB2Periph_GPIOB)

#define PWM1_GPIO_0 GPIOA
#define PWM1_GPIO_1 GPIOD

#define PWM2_GPIO_0 GPIOB

#define PWM1_GPIO_1_PIN0 GPIO_Pin_10
#define PWM1_GPIO_1_PIN1 GPIO_Pin_11

#define PWM2_GPIO_1_PIN0 GPIO_Pin_8
#define PWM2_GPIO_1_PIN1 GPIO_Pin_9


#define PWM1_GPIO_0_PIN0 GPIO_Pin_6

#define PWM2_GPIO_0_PIN0 GPIO_Pin_6

#define Motor1_Star   GPIO_WriteBit(PWM1_GPIO_1, PWM1_GPIO_1_PIN0, Bit_SET)
#define Motor1_Stop   GPIO_WriteBit(PWM1_GPIO_1, PWM1_GPIO_1_PIN0, Bit_RESET)
#define Motor1_Zheng  GPIO_ResetBits(PWM1_GPIO_1, PWM1_GPIO_1_PIN1)
#define Motor1_Fan    GPIO_SetBits(PWM1_GPIO_1, PWM1_GPIO_1_PIN1)

#define Motor2_Star   GPIO_WriteBit(PWM1_GPIO_1, PWM2_GPIO_1_PIN0, Bit_SET)
#define Motor2_Stop   GPIO_WriteBit(PWM1_GPIO_1, PWM2_GPIO_1_PIN0, Bit_RESET)
#define Motor2_Zheng  GPIO_ResetBits(PWM1_GPIO_1, PWM2_GPIO_1_PIN1)
#define Motor2_Fan    GPIO_SetBits(PWM1_GPIO_1, PWM2_GPIO_1_PIN1)

#define SSB_XiFen (128)

/*******************函数声明区*****************************/
extern void PWM1_OUT_Config(void);
static void PWM1_GPIO_Config(void);
static void PWM1_Mode_Config(void);
static void PWM1_NVIC_Config(void);
extern void PWM1_Motor_Jiao(u32 a);

extern void PWM2_OUT_Config(void);
static void PWM2_GPIO_Config(void);
static void PWM2_Mode_Config(void);
static void PWM2_NVIC_Config(void);
extern void PWM2_Motor_Jiao(u32 a);
extern void PWM12_Motor_Jiao(u32 jiao1, u32 jiao2);


#endif
