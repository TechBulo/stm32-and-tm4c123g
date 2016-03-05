/******************************************************************************
*
���˵�����ó���ʵ����STM32��Ƭ������4*4��Ĥ���̵�һϵ�в�������ͨ��������ϵͳ�ɵõ�
          ��ǰ�İ������ݡ�
ע������ó���Ϊ�û��ṩ�˷���ֵΪchar��ENTER_Key�����������ú������Է��ذ�����ֵ��
          �ڵ��øÿ�ʱ�������KEY_GPIO_Config��������4*4���̵����á�                        
                          
�ӿڶ��壺a0-a8���Ը���(��a6�⣬��������̵�8���˿�����)
******************************************************************************/

#ifndef _KEY_H_
#define _KEY_H_

/***********ͷ�ļ���************/
#include <stm32f10x.h>
#include <delay.h>

/***********������ֲ��************/
#define RCC_APB2Periph_GPIO_KEY RCC_APB2Periph_GPIOA
#define GPIO_KEY   GPIOA
#define GPIO_Key_0 GPIO_Pin_0
#define GPIO_Key_1 GPIO_Pin_1
#define GPIO_Key_2 GPIO_Pin_2
#define GPIO_Key_3 GPIO_Pin_3
#define GPIO_Key_4 GPIO_Pin_4
#define GPIO_Key_5 GPIO_Pin_5
#define GPIO_Key_6 GPIO_Pin_7
#define GPIO_Key_7 GPIO_Pin_8

/***********����������************/
extern void KEY_GPIO_Config(void);
extern char ENTER_Key(void);

/***********�ṹ�嶨����************/
typedef struct Key_Str
{
    GPIO_TypeDef *GPIO_X;
    uint16_t GPIO_Pin;
}Key_Str;

static Key_Str Key_Out[4] =  
{
    {GPIO_KEY,GPIO_Key_0},{GPIO_KEY,GPIO_Key_1},
    {GPIO_KEY,GPIO_Key_2},{GPIO_KEY,GPIO_Key_3}
};

static Key_Str Key_Int[4] = 
{
    {GPIO_KEY,GPIO_Key_4},{GPIO_KEY,GPIO_Key_5},
    {GPIO_KEY,GPIO_Key_6},{GPIO_KEY,GPIO_Key_7}
} ;

static char Key[4][4] = 
{
    'D' , 'C', 'B', 'A',
    '#' , '9', '6', '3',
    '0' , '8', '5', '2',
    '*' , '7', '4', '1',
};

#endif
