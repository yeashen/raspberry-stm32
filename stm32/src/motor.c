/******************************************************************************

	Copyright (C), 20015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : motor.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2017/07/01
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2017/07/01
   Author       : Juven
   Modification : Created file

******************************************************************************/

#include "motor.h"

/*
 * PB.10 - IN1
 * PB.11 - IN2
 * PB.12 - IN3
 * PB.13 - IN4
 */
void motor_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
}

void motor_direction_ctrl(u8 side, u8 dir)
{
	if(side == LEFT){
		if(dir == FOREWARD){
			GPIO_SetBits(GPIOB, GPIO_Pin_10);
			GPIO_ResetBits(GPIOB, GPIO_Pin_11);
		}else if(dir == REVERSAL){
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			GPIO_SetBits(GPIOB, GPIO_Pin_11);
		}else if(dir == STOP){
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			GPIO_ResetBits(GPIOB, GPIO_Pin_11);
		}else{
			GPIO_SetBits(GPIOB, GPIO_Pin_10);
			GPIO_SetBits(GPIOB, GPIO_Pin_11);
		}
	}else if(side == RIGHT){
		if(dir == FOREWARD){
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		}else if(dir == REVERSAL){
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
		}else if(dir == STOP){
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		}else{
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
		}
	}
}

