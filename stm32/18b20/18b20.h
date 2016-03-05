#ifndef _18B20_H_
#define _18B20_H_

/***********头文件区************/
#include <stm32f10x.h>
#include "delay.h"

/***********代码移植区************/
#define RCC_APB2Periph_GPIO_18B20 RCC_APB2Periph_GPIOB
#define GPIO_18B20_Pin GPIO_Pin_6
#define GPIO_18B20     GPIOB

/***********宏定义区************/
#define DATA_IN  1
#define DATA_OUT 0
#define DS18B20_Pin_L GPIO_ResetBits(GPIO_18B20, GPIO_18B20_Pin)
#define DS18B20_Pin_H GPIO_SetBits(GPIO_18B20, GPIO_18B20_Pin)

/***********函数声明区************/
extern void DS18B20_RCC(void);
static void GPIO_18B20_Confing(u8 way);
static ErrorStatus Init_18B20(void);
static u8 Read_Byte_18B20(void);
static void Write_Byte_18B20(u8 data);
static void Skip_ID(void);
static void Change(void);
static void Read_Ram(void);
static u16 Tmp_Get(void);
extern u16 One_Change(void);

#endif
