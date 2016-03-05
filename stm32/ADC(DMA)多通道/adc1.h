/******************************************************************************
*
相关说明：该程序实现了STM32单片ADC1的一系列操作可以实现多通道ad转换
注意事项：该程序为用户提供了返回值为 float GetValue(u16 advalue)函数，该函数可以
          返回AD转换后的实际电压值，在调用该库时必须调用ADC1_Init进行ADC的配置。                        
                          
接口定义：以下是各个通道的接口，程序默认打开所有的，要用到那些通道自己要进行连接，
		  在未用到的接口，最好自己在程序中将其删除，以免产生不必要的错误
******************************************************************************/

/*GPIO管脚的配置
选用ADC的通道0    1     2   8    9   10    11   12    13    14  15，分别对应的管脚为
            PA0  PA1  PA2  PB0  PB1  PC0  PC1  PC2  PC3  PC4  PC5
串口使用USART1其中TX为PA9，RX为PA10 */


#ifndef _ADC1_DUO_LU_H_
#define _ADC1_DUO_LU_H_

/*****************头文件************************/

#include <stm32f10x.h>

/*****************宏定义区*********************/
/*******若要更改通道数，必须修改此处***********/
#define ADC_TONGDAO_NUM 11

/*****************函数声明区*********************/
static void ADC1_GPIO_Config(void);
static void ADC1_Config(void);
static void DMA_Config(void);
extern void ADC1_Init(void);
extern float GetValue(u16 advalue);//直接返回实际电压

#endif
