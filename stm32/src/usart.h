#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/

void uart_init(u32 bound);

int uart1_send(const u8 *buf, u32 len);

int printf(const char *format, ...);

int sprintf(char *out, const char *format, ...);

int snprintf( char *buf, unsigned int count, const char *format, ... );

#endif


