#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#include "usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
 				  
void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节

#define SPI_PDEBUG
#ifdef SPI_PDEBUG
#define SPI_DEBUG(string,args...)	printf("[SPI_DEBUG] ");	\
									printf(string, ##args)
#else
#define SPI_DEBUG(string,args...)
#endif
		 
#endif

