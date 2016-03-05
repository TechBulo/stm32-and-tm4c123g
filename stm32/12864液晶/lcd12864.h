/******************************************************************************
*
���˵�����ó���ʵ����STM32��Ƭ������12864Һ����һϵ�в�����
ע������ó���Ϊ�û��ṩ�˷���ֵΪchar��ENTER_Key�����������ú������Է��ذ�����ֵ��
          �ڵ��øÿ�ʱ�������KEY_GPIO_Config��������4*4���̵����á�                        
                          
�ӿڶ��壺IO��ռ�������PD0��RS   	D1��R/W  	D2��EN		D3��Reset		PD4��Vout��
		  VSS�ӵأ�VDD��5V��ѹ��	����ƣ�A������K�Ӹ���5V��ѹ��
******************************************************************************/
#ifndef __LCD12864_H
#define __LCD12864_H

/***********ͷ�ļ���************/
#include <stm32f10x.h>
#include "delay.h"

/***********������ֲ��************/
#define LCD12864_GPIO		(GPIOD)
#define LCD12864_CS_PIN		(GPIO_Pin_0)	//Ƭѡ�ź�
#define LCD12864_SID_PIN	(GPIO_Pin_1)	//�����ź�
#define LCD12864_SCLK_PIN	(GPIO_Pin_2)	//ʱ���ź�
#define LCD12864_RST_PIN	(GPIO_Pin_3)	//��λ�ź�
#define LCD12864_CH_PIN		(GPIO_Pin_4)	//����/���� ѡ���ź�
#define RCC_APB2Periph_LCD12864_GPIO RCC_APB2Periph_GPIOD//ʱ��

/***********����������************/
static void Send_Byte(unsigned char data);
static void Write_Data(unsigned char disdata);
extern void LCD_Display(char *string);
static void LCD_init(void);
extern void LCD_GPIO_Config(void);//��ʼ������
static void Write_Com(unsigned char cmdcode);
static void LCD_Example(void);
static void LCD_CLC(void);
extern void LCD_POS(unsigned char x, unsigned char y);
extern void LCD_Num(int value)	;//Һ����ʾ����
static char *itoa(int value, char *string, int radix);
extern void LCD_FNum(float fvalue);//Һ����ʾ������
static int ftoa(char *str, float num, int n);


#endif  /*__LCD12864_H*/

