/******************************************************************************
*
���˵�����ó���ʵ����STM32��Ƭ�����ƴ��ڵ�һϵ�в���
                          ������������û������жϽ��յķ������
                          ��Ҫʹ�øĳ�����Ҫע����ȫ�ֱ�������һ��
                          READ_Buf[]���鲢������ע�͵Ĵ��ڷ��������뵽
                          Stm32f10x_it.c��
�ӿڶ��壺
******************************************************************************/

#ifndef _USART_H_
#define  _USART_H_

#include<stm32f10x.h>
#include<stdio.h>
#include <stdarg.h>

void USART1_Config (void);//��TXΪA9,RXΪA10
void USART2_Confing(void);//��TXΪA2��RXΪA3
int fputc(int ch,FILE *f);
char * itoa(int value,char * string,int radix);
void USART_printf(USART_TypeDef * USARTx,uint8_t *Data,...);
void NVIC_USART2_Config(void);


#endif
