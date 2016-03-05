#include "sonic.h"

extern u32 Distance;

void Sonic_Config(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
						    RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);
	NVIC_Configuration();
	Sonic_GPIO_Configuration();
	Sonic_EXTI_Configuration();
	Timer_Configuration();
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)1);
}

static void Sonic_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void Timer_Configuration(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

	TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Period=60000;		 //ARR的值, 一次计时6秒
	TIM_TimeBaseStructure.TIM_Prescaler=72;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
}

static void Sonic_EXTI_Configuration(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/* Configure PB.09 pin as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Connect EXTI9 Line to PB.09 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
	/* Configure EXTI9 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI9_5 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//最高优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

static void NVIC_Configuration(void)
{  
	#ifdef  VECT_TAB_RAM
	  /* Set the Vector Table base location at 0x20000000 */
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);          //选择中断分组2
}

void Sonic_Start(void)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)0);//低电平触发
	Sonic_Delay_1ms(1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)1);//恢复高电平
}

static void Sonic_Delay_1ms(u32 time)
{
   u16 i=0;
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;
   }
}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
******************************************************************************

volatile u32 Distance;

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line12);

		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
		{
			TIM_SetCounter(TIM1,0);
			TIM_Cmd(TIM1, ENABLE);
		}
		else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
		{
			TIM_Cmd(TIM1, DISABLE);
			Distance=(TIM_GetCounter(TIM1)*0.17);
		}
	}
}
*/
