#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#ifndef DEBUG_UART
#define DEBUG_UART USART1
#endif
	
void uart1_init(u32 bound);

void uart2_init(u32 bound);

void uart3_init(u32 bound);

int uart1_send(const u8 *buf, u32 len);

int uart3_send(const u8 *buf, u32 len);

int printf(const char *format, ...);

int sprintf(char *out, const char *format, ...);

int snprintf( char *buf, unsigned int count, const char *format, ... );
#endif


