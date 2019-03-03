/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : usart.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : UART1 Funcitons
  Function List :
              fputc
              _sys_exit
              uart_init
              USART1_IRQHandler
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#include "sys.h"
#include "usart.h"
#include "pkthandle.h"
#include <stdarg.h> 

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

void printchar(char **str, unsigned int c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, (uint8_t)c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
    register int pc = 0, padchar = ' ';

    if (width > 0) {
        register int len = 0;
        register const char *ptr;
        for (ptr = string; *ptr; ++ptr) ++len;
        if (len >= width) width = 0;
        else width -= len;
        if (pad & PAD_ZERO) padchar = '0';
    }
    if (!(pad & PAD_RIGHT)) {
        for ( ; width > 0; --width) {
            printchar (out, padchar);
            ++pc;
        }
    }
    for ( ; *string ; ++string) {
        printchar (out, *string);
        ++pc;
    }
    for ( ; width > 0; --width) {
        printchar (out, padchar);
        ++pc;
    }

    return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
    char print_buf[PRINT_BUF_LEN];
    register char *s;
    register int t, neg = 0, pc = 0;
    register unsigned int u = i;

    if (i == 0) {
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return prints (out, print_buf, width, pad);
    }

    if (sg && b == 10 && i < 0) {
        neg = 1;
        u = -i;
    }

    s = print_buf + PRINT_BUF_LEN-1;
    *s = '\0';

    while (u) {
        t = u % b;
        if( t >= 10 )
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= b;
    }

    if (neg) {
        if( width && (pad & PAD_ZERO) ) {
            printchar (out, '-');
            ++pc;
            --width;
        }
        else {
            *--s = '-';
        }
    }

    return pc + prints (out, s, width, pad);
}

static int print( char **out, const char *format, va_list args )
{
    register int width, pad;
    register int pc = 0;
    char scr[2];

    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            width = pad = 0;
            if (*format == '\0') break;
            if (*format == '%') goto out;
            if (*format == '-') {
                ++format;
                pad = PAD_RIGHT;
            }
            while (*format == '0') {
                ++format;
                pad |= PAD_ZERO;
            }
            for ( ; *format >= '0' && *format <= '9'; ++format) {
                width *= 10;
                width += *format - '0';
            }
            if( *format == 's' ) {
                register char *s = (char *)va_arg( args, int );
                pc += prints (out, s?s:"(null)", width, pad);
                continue;
            }
            if( *format == 'd' ) {
                pc += printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
                continue;
            }
            if( *format == 'x' ) {
                pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
                continue;
            }
            if( *format == 'X' ) {
                pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
                continue;
            }
            if( *format == 'u' ) {
                pc += printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
                continue;
            }
            if( *format == 'c' ) {
                /* char are converted to int then pushed on the stack */
                scr[0] = (char)va_arg( args, int );
                scr[1] = '\0';
                pc += prints (out, scr, width, pad);
                continue;
            }
        }
        else {
            out:
            printchar (out, *format);
            ++pc;
        }
    }
    if (out) **out = '\0';
    va_end( args );
    return pc;
}

int printf(const char *format, ...)
{
    va_list args;

    va_start( args, format );
    return print( 0, format, args );
}

int sprintf(char *out, const char *format, ...)
{
    va_list args;

    va_start( args, format );
    return print( &out, format, args );
}

int snprintf( char *buf, unsigned int count, const char *format, ... )
{
    va_list args;

    ( void ) count;

    va_start( args, format );
    return print( &buf, format, args );
}

int uart1_send(const u8 *buf, u32 len)
{
	int i = 0;

	for(i = 0; i < len; i++){
		while((USART1->SR&0X40)==0);
    	USART1->DR = (u8) buf[i];
	}

	return i;
}


void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef stGpio_Init;
	USART_InitTypeDef stUart_Init;
	NVIC_InitTypeDef stNvic_Init;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
	stGpio_Init.GPIO_Pin = GPIO_Pin_9; //PA.9
	stGpio_Init.GPIO_Speed = GPIO_Speed_50MHz;
	stGpio_Init.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &stGpio_Init);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	stGpio_Init.GPIO_Pin = GPIO_Pin_10;//PA10
	stGpio_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &stGpio_Init);//初始化GPIOA.10  
	//USART 初始化设置

	//Usart1 NVIC 配置
	stNvic_Init.NVIC_IRQChannel = USART1_IRQn;
	stNvic_Init.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	stNvic_Init.NVIC_IRQChannelSubPriority = 1;		//子优先级
	stNvic_Init.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&stNvic_Init);	//根据指定的参数初始化VIC寄存器

	stUart_Init.USART_BaudRate = bound;//串口波特率
	stUart_Init.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	stUart_Init.USART_StopBits = USART_StopBits_1;//一个停止位
	stUart_Init.USART_Parity = USART_Parity_No;//无奇偶校验位
	stUart_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	stUart_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &stUart_Init); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

static u8 recving = 0;
static u8 count = 0;
static u8 num = 0;
static uart_pkt_s pkt;

/**************************************************************************
函数功能：串口1接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART1_IRQHandler(void)
{	
	u8 dat;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
	{	
		dat = USART_ReceiveData(USART1);//(USART1->DR);
		//printf("%x \r\n", dat);	
		if((!recving) && (dat == PACKET_START)){
			recving = 1;
			count++;
			pkt.start = dat;
			return;
		}

		if(recving == 1){
			if(count == 1){/* type */
     			pkt.type = dat;
			}else if(count == 2){/* command */
     			pkt.cmd = dat;
			}else if(count == 3){/* len */
     			pkt.len = dat;
			}else if(count == (pkt.len+4)){/* checksum lsb 8bit*/
     			pkt.checksum = dat;
			}else if(count == (pkt.len+5)){/* checksum msb 8bit */
     			pkt.checksum |= (dat<<8);
				recving = 0;
				count = 0;
				num = 0;
				packet_handle(&pkt);
				return;
			}else{/* data*/
				if(num < pkt.len){
     				pkt.params[num++] = dat;
				}
			}
			count++;
		}
	}										 
} 


