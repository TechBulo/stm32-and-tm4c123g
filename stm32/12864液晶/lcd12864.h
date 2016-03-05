/******************************************************************************
*
相关说明：该程序实现了STM32单片机控制12864液晶的一系列操作。
注意事项：该程序为用户提供了返回值为char的ENTER_Key（）函数，该函数可以返回按键的值，
          在调用该库时必须调用KEY_GPIO_Config（）进行4*4键盘的配置。                        
                          
接口定义：IO口占用情况：PD0：RS   	D1：R/W  	D2：EN		D3：Reset		PD4：Vout；
		  VSS接地，VDD接5V电压；	背光灯：A接正，K接负，5V电压；
******************************************************************************/
#ifndef __LCD12864_H
#define __LCD12864_H

/***********头文件区************/
#include <stm32f10x.h>
#include "delay.h"

/***********代码移植区************/
#define LCD12864_GPIO		(GPIOD)
#define LCD12864_CS_PIN		(GPIO_Pin_0)	//片选信号
#define LCD12864_SID_PIN	(GPIO_Pin_1)	//数据信号
#define LCD12864_SCLK_PIN	(GPIO_Pin_2)	//时钟信号
#define LCD12864_RST_PIN	(GPIO_Pin_3)	//复位信号
#define LCD12864_CH_PIN		(GPIO_Pin_4)	//串行/并行 选择信号
#define RCC_APB2Periph_LCD12864_GPIO RCC_APB2Periph_GPIOD//时钟

/***********函数声明区************/
static void Send_Byte(unsigned char data);
static void Write_Data(unsigned char disdata);
extern void LCD_Display(char *string);
static void LCD_init(void);
extern void LCD_GPIO_Config(void);//初始化配置
static void Write_Com(unsigned char cmdcode);
static void LCD_Example(void);
static void LCD_CLC(void);
extern void LCD_POS(unsigned char x, unsigned char y);
extern void LCD_Num(int value)	;//液晶显示数字
static char *itoa(int value, char *string, int radix);
extern void LCD_FNum(float fvalue);//液晶显示浮点数
static int ftoa(char *str, float num, int n);


#endif  /*__LCD12864_H*/

