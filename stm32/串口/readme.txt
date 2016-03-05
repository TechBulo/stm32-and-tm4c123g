串口驱动使用注意事项：

1.IO口占用情况：串口1其TX为A9,RX为A10，串口2其TX为A2，RX为A3。

2.main函数中需要添加的头文件：

#include "usart.h"

3.串口1使用时必须调用USART1_Config（）进行初始化设置。为串口1提供了一下函数
  printf（）；//标准输入输出。
  USART_printf(USART1,“数据为%d  %s  \r\n\\”,10，“adc”)//自定义输入输出10   adc  \
  串口2使用时必须调用USART2_Config（）进行配置。
  为串口2还提供了中断接收使用时要调用NVIC_USART2_Config进行初始化配置，
  注意在全局变量定义一个READ_Buf[]数组并将程序中的注释语句即的串口服务程序加入到  Stm32f10x_it.c中 
  USART_printf(USART1,“数据为%d  %s  \r\n\\”,10，“adc”)//自定义输入输出10   adc  \