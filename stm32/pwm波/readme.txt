步进电机驱动使用注意事项：

1.IO口占用情况：PA6：脉冲输出端;  PD11：DIR+；	PD10：EN+使能端
		PB6:电机2的脉冲输出端。PD9：DIR+；PD8：EN+

2.驱动板采用共阴极接法，CP-、EN-、和DIR-都接地；

3.main函数中需要添加的头文件：

	#include "pwm_out.h"

4.main函数中定义的全局变量或者引用的全局变量：

volatile u32 Motor_Jiao_1=0;
volatile u32 Motor_Jiao_2=0;

5.需在"stm32f10x_it.h"中添加
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

需要在"stm32f10x_it.c"添加以下
extern u32 Motor_Jiao_1;
extern u32 Motor_Jiao_2;
volatile char IT_Flag_1 = 0;
volatile char IT_Flag_2 = 0;

void TIM3_IRQHandler(void)
{
    static u32 time_1 = 0;
	if ( TIM_GetITStatus(PWM1_TIM , TIM_IT_Update) != RESET )
	{
		TIM_ClearITPendingBit(PWM1_TIM , TIM_FLAG_Update);
       //printf("123");
  		 time_1++;
         //printf("%d  ",time);
	}
    //jiao = jiao/1.8;
	if(time_1>Motor_Jiao_1)
	{
    printf("jiao wei %d",Motor_Jiao_1);
	  TIM_Cmd(PWM1_TIM, DISABLE);                   //禁能定时器
	  IT_Flag_1=1;
      Motor1_Stop;
      time_1 = 0;
	}
}

void TIM4_IRQHandler(void)
{
    static u32 time_2=0;
	if ( TIM_GetITStatus(PWM2_TIM , TIM_IT_Update) != RESET )
	{
		TIM_ClearITPendingBit(PWM2_TIM , TIM_FLAG_Update);
       //printf("123");
  		 time_2++;
        // printf("%d  ",time_2);
	}
    //jiao = jiao/1.8;
	if(time_2>Motor_Jiao_2)
	{
    // printf("jiao wei %d",Motor_Jiao_2);
	  TIM_Cmd(PWM2_TIM, DISABLE);                   //禁能定时器
	  IT_Flag_2=1;
      Motor2_Stop;
      time_2 = 0;
	}
}



6.使用前应调用的初始化函数：
    PWM1_OUT_Config();或者    PWM2_OUT_Config();
然后可以使用    PWM1_Motor_Jiao(90); 或   PWM2_Motor_Jiao(90);或PWM12_Motor_Jiao(90，90)让两个电机同时工作;
也可调用
 Motor1_Star//电机一启动
 Motor1_Stop//停止
 Motor1_Zheng //正转
 Motor1_Fan  //反转

 Motor2_Star   
 Motor2_Stop   
 Motor2_Zheng  
 Motor2_Fan    



