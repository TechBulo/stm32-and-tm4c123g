�����������ʹ��ע�����

1.IO��ռ�������PA6�����������;  PD11��DIR+��	PD10��EN+ʹ�ܶ�
		PB6:���2����������ˡ�PD9��DIR+��PD8��EN+

2.��������ù������ӷ���CP-��EN-����DIR-���ӵأ�

3.main��������Ҫ��ӵ�ͷ�ļ���

	#include "pwm_out.h"

4.main�����ж����ȫ�ֱ����������õ�ȫ�ֱ�����

volatile u32 Motor_Jiao_1=0;
volatile u32 Motor_Jiao_2=0;

5.����"stm32f10x_it.h"�����
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

��Ҫ��"stm32f10x_it.c"�������
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
	  TIM_Cmd(PWM1_TIM, DISABLE);                   //���ܶ�ʱ��
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
	  TIM_Cmd(PWM2_TIM, DISABLE);                   //���ܶ�ʱ��
	  IT_Flag_2=1;
      Motor2_Stop;
      time_2 = 0;
	}
}



6.ʹ��ǰӦ���õĳ�ʼ��������
    PWM1_OUT_Config();����    PWM2_OUT_Config();
Ȼ�����ʹ��    PWM1_Motor_Jiao(90); ��   PWM2_Motor_Jiao(90);��PWM12_Motor_Jiao(90��90)���������ͬʱ����;
Ҳ�ɵ���
 Motor1_Star//���һ����
 Motor1_Stop//ֹͣ
 Motor1_Zheng //��ת
 Motor1_Fan  //��ת

 Motor2_Star   
 Motor2_Stop   
 Motor2_Zheng  
 Motor2_Fan    



