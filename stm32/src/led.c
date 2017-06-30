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
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void led1_set(unsigned char sw)
{
	if(sw == ON)
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
	else
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
}

void led2_set(unsigned char sw)
{
	if(sw == ON)
		GPIO_SetBits(GPIOG, GPIO_Pin_14);
	else
		GPIO_ResetBits(GPIOG, GPIO_Pin_14);
}

