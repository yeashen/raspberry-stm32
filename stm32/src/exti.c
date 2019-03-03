/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : exti.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : exit int functions
  Function List :
              robot_exit_init
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#include "exti.h"

/**************************************************************************
函数功能：外部中断初始化
入口参数：无
返回  值：无 
**************************************************************************/
void robot_exit_init(void)
{  
	GPIO_InitTypeDef stGpio_Init;
	EXTI_InitTypeDef stExit_Init;
	NVIC_InitTypeDef stNvic_Init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIO端口时钟
	stGpio_Init.GPIO_Pin = GPIO_Pin_12;	            //端口配置
	stGpio_Init.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
	GPIO_Init(GPIOA, &stGpio_Init);					      //根据设定参数初始化GPIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	stExit_Init.EXTI_Line=EXTI_Line12;
	stExit_Init.EXTI_Mode = EXTI_Mode_Interrupt;	
	stExit_Init.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	stExit_Init.EXTI_LineCmd = ENABLE;
	EXTI_Init(&stExit_Init);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	stNvic_Init.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
	stNvic_Init.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	stNvic_Init.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
	stNvic_Init.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&stNvic_Init); 
}










