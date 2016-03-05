#include <pwm_out.h>
#include <usart.h>

extern u32 Motor_Jiao_1 ;
extern u32 Motor_Jiao_2;

extern u32 IT_Flag_1 ;
extern u32 IT_Flag_2 ;

static void PWM1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitSturcture;
     TIM_ARRPreloadConfig(PWM1_TIM, ENABLE);			 // 使能TIM3重载寄存器ARR

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_PWM1_GPIO_0|RCC_APB2Periph_PWM1_GPIO_1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWM1_TIM, ENABLE);

    GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitSturcture.GPIO_Pin = PWM1_GPIO_0_PIN0;
    GPIO_Init(PWM1_GPIO_0, &GPIO_InitSturcture);

    GPIO_InitSturcture.GPIO_Pin = PWM1_GPIO_1_PIN0|PWM1_GPIO_1_PIN1;
    GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PWM1_GPIO_1, &GPIO_InitSturcture);
}

static void PWM2_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitSturcture;
     TIM_ARRPreloadConfig(PWM2_TIM, ENABLE);			 // 使能TIM3重载寄存器ARR

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_PWM2_GPIO_0|RCC_APB2Periph_PWM1_GPIO_1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWM2_TIM, ENABLE);

    GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitSturcture.GPIO_Pin = PWM2_GPIO_0_PIN0;
    GPIO_Init(PWM2_GPIO_0, &GPIO_InitSturcture);

    GPIO_InitSturcture.GPIO_Pin = PWM2_GPIO_1_PIN0|PWM2_GPIO_1_PIN1;
    GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PWM1_GPIO_1, &GPIO_InitSturcture);
}


static void PWM1_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitSturcture;
    TIM_OCInitTypeDef TIM_OCInitSturcture;

    u16 CCR1_Val = 1250;

    TIM_DeInit(PWM1_TIM);
    TIM_TimeBaseInitSturcture.TIM_Prescaler = (36-1);
    TIM_TimeBaseInitSturcture.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitSturcture.TIM_Period = 2500;
    TIM_TimeBaseInitSturcture.TIM_ClockDivision = 0;

    TIM_TimeBaseInit(PWM1_TIM,&TIM_TimeBaseInitSturcture);

    TIM_ClearFlag(PWM1_TIM, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(PWM1_TIM,TIM_IT_Update,ENABLE);

   TIM_OCInitSturcture.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
   TIM_OCInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitSturcture.TIM_Pulse = CCR1_Val;
//设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitSturcture.TIM_OCPolarity = TIM_OCPolarity_High;
//当定时器计数值小于CCR1_Val时为高电平

  TIM_OC1Init(PWM1_TIM, &TIM_OCInitSturcture);	 //使能通道1

    TIM_OC1PreloadConfig(PWM1_TIM, TIM_OCPreload_Disable);
     TIM_ARRPreloadConfig(PWM1_TIM, ENABLE);			 // 使能TIM3重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(PWM1_TIM, DISABLE);                   //使能定时器3

}

static void PWM2_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitSturcture;
    TIM_OCInitTypeDef TIM_OCInitSturcture;

    u16 CCR1_Val = 1250;

    TIM_DeInit(PWM2_TIM);
    TIM_TimeBaseInitSturcture.TIM_Prescaler = (36-1);
    TIM_TimeBaseInitSturcture.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitSturcture.TIM_Period = 2500;
    TIM_TimeBaseInitSturcture.TIM_ClockDivision = 0;

    TIM_TimeBaseInit(PWM2_TIM,&TIM_TimeBaseInitSturcture);

    TIM_ClearFlag(PWM2_TIM, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(PWM2_TIM,TIM_IT_Update,ENABLE);

   TIM_OCInitSturcture.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
   TIM_OCInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitSturcture.TIM_Pulse = CCR1_Val;
//设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitSturcture.TIM_OCPolarity = TIM_OCPolarity_High;
//当定时器计数值小于CCR1_Val时为高电平

  TIM_OC1Init(PWM2_TIM, &TIM_OCInitSturcture);	 //使能通道1

    TIM_OC1PreloadConfig(PWM2_TIM, TIM_OCPreload_Disable);
     TIM_ARRPreloadConfig(PWM2_TIM, ENABLE);			 // 使能TIM3重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(PWM2_TIM, DISABLE);                   //使能定时器3

}

static void PWM2_NVIC_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void PWM1_NVIC_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

extern void PWM1_OUT_Config(void)
{
    PWM1_NVIC_Config();
    PWM1_GPIO_Config();
    PWM1_Mode_Config();
    Motor1_Stop;
   // Motor1_Zheng;
}

extern void PWM2_OUT_Config(void)
{
    PWM2_NVIC_Config();
    PWM2_GPIO_Config();
    PWM2_Mode_Config();
    Motor2_Stop;
   // Motor1_Zheng;
}

extern void PWM1_Motor_Jiao(u32 a)
{
     Motor_Jiao_1 = (a*SSB_XiFen)/1.8;
     TIM_OC1PreloadConfig(PWM1_TIM, TIM_OCPreload_Enable);
     TIM_Cmd(PWM1_TIM, ENABLE);//中断中关了定时器，现在要打开
    // printf("角度是%d",Motor_Jiao_1);
     while(!IT_Flag_1);
     IT_Flag_1 = 0;
     //Motor1_Star;//如果输入角度结束则启动电机
}

extern void PWM2_Motor_Jiao(u32 a)
{
     Motor_Jiao_2 = (a*SSB_XiFen)/1.8;
     TIM_OC1PreloadConfig(PWM2_TIM, TIM_OCPreload_Enable);
     TIM_Cmd(PWM2_TIM, ENABLE);//中断中关了定时器，现在要打开
     //printf("角度是%d",Motor_Jiao_2);
     while(!IT_Flag_2);
     IT_Flag_2 = 0;
     //Motor1_Star;//如果输入角度结束则启动电机
}

extern void PWM12_Motor_Jiao(u32 jiao1, u32 jiao2)
{
    Motor_Jiao_1 = ( a*SSB_XiFen ) / 1.8;
    Motor_Jiao_2 = ( a*SSB_XiFen ) / 1.8;
    TIM_OC1PreloadConfig(PWM1_TIM, TIM_OCPreload_Enable);
    TIM_Cmd( PWM1_TIM , ENABLE);
    TIM_OC1PreloadConfig(PWM2_TIM, TIM_OCPreload_Enable);
    TIM_Cmd( PWM2_TIM , ENABLE);

     while(!IT_Flag_1);
       IT_Flag_1 = 0;

    while(!IT_Flag_2);
      IT_Flag_2 = 0;
}



