/******************************************************************************
*
相关说明：该程序实现了STM32单片机控制串口的一系列操作
                          本程序给出了用户串口中断接收的服务程序
                          若要使用改程序则要注意在全局变量定义一个
                          READ_Buf[]数组并将下面注释的串口服务程序加入到
                          Stm32f10x_it.c中
接口定义：
******************************************************************************/

#ifndef _USART_H_
#define  _USART_H_

#include<stm32f10x.h>
#include<stdio.h>
#include <stdarg.h>

void USART1_Config (void);//其TX为A9,RX为A10
void USART2_Confing(void);//其TX为A2，RX为A3
int fputc(int ch,FILE *f);
char * itoa(int value,char * string,int radix);
void USART_printf(USART_TypeDef * USARTx,uint8_t *Data,...);
void NVIC_USART2_Config(void);


#endif
