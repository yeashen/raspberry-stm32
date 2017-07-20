/******************************************************************************

	Copyright (C), 2015-2025, DIY Co., Ltd.

 ******************************************************************************
  File Name     : led.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2017/06/17
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2017/06/17
   Author       : Juven
   Modification : Created file

******************************************************************************/

#include "led.h"

void led_init()
{
	/*
	 * PA3 -> LED_R
	 * PA5 -> LED_G
	 * PB0 -> LED_B
	 */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);
#if 0//!(DEBUG_UART == 2)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
#if 0//!(DEBUG_UART == 2)
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
#endif
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void led_rgb_set(u8 rgb)
{
	switch(rgb){
		case LED_RED:
#if 0//!(DEBUG_UART == 2)
			GPIO_ResetBits(GPIOA, GPIO_Pin_3);
#endif
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
			break;
		case LED_GREEN:
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
#if 0//!(DEBUG_UART == 2)
			GPIO_SetBits(GPIOA, GPIO_Pin_3);
#endif
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
			break;
		case LED_BLUE:
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#if 0//!(DEBUG_UART == 2)
			GPIO_SetBits(GPIOA, GPIO_Pin_3);
#endif
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			break;
	}
}

void led_set(u8 sw)
{
	if(sw == LED_OFF){
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}else{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	}
}


