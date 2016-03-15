/*
 * My_uart.h
 *
 *  Created on: 2014Äê7ÔÂ28ÈÕ
 *      Author: Itheaven
 */

#ifndef MY_UART_H_
#define MY_UART_H_
/************
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "My_uart.h"

#include"stdio.h"
#include<stdint.h>
#include<stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"

void UART1_INIT(void);
void UART1_send_string(const char *sendbuf);
void UART1_send_char(const char Byte);
char UART1_recive_char(void);
void UART1_recive_string(char *reciveBuf,int num);
void UART1_send_int16_t_string(const int16_t *temp, uint64_t num);
void UART1_send_int32_t_string(const int32_t *temp, uint64_t num);
void UART1_send_int64_t_string(const int64_t *temp, uint64_t num);
void UART1_send_int16_t(const int16_t temp,char on);
void UART1_send_int32_t(const int32_t temp,char on);
void UART1_send_int64_t(const int64_t temp,char on);
void UART1_send_float(const float temp,int64_t lenth);
void UART1_send_char_string(const unsigned char *temp, uint64_t num);
void UART1_send_int64_t_string(const int64_t *temp, uint64_t num);
//void UART0_INIT(void);
void UART0_init(void);
void UART0_send_char(const char Byte);
void UART0_send_string(const char *sendbuf);
#endif /* MY_UART_H_ */
