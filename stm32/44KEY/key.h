/******************************************************************************
*
相关说明：该程序实现了STM32单片机控制4*4薄膜键盘的一系列操作可以通过按键，系统可得到
          当前的按键内容。
注意事项：该程序为用户提供了返回值为char的ENTER_Key（）函数，该函数可以返回按键的值，
          在调用该库时必须调用KEY_GPIO_Config（）进行4*4键盘的配置。                        
                          
接口定义：a0-a8可以更换(除a6外，依次与键盘的8个端口相连)
******************************************************************************/

#ifndef _KEY_H_
#define _KEY_H_

/***********头文件区************/
#include <stm32f10x.h>
#include <delay.h>

/***********代码移植区************/
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

/***********函数声明区************/
extern void KEY_GPIO_Config(void);
extern char ENTER_Key(void);

/***********结构体定义区************/
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
