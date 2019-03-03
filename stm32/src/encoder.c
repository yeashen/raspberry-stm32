/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : encoder.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : encoder speed
  Function List :
              encoder_init_tim2
              encoder_init_tim4
              read_encoder
              TIM2_IRQHandler
              TIM4_IRQHandler
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#include "encoder.h"

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void encoder_init_tim2(void)
{
	TIM_TimeBaseInitTypeDef stTimerBase_Init;  
	TIM_ICInitTypeDef stTimerIc_Init;  
	GPIO_InitTypeDef stGpio_Init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器4的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PB端口时钟

	stGpio_Init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//端口配置
	stGpio_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &stGpio_Init);					      //根据设定参数初始化GPIOB

	TIM_TimeBaseStructInit(&stTimerBase_Init);
	stTimerBase_Init.TIM_Prescaler = 0x0; // 预分频器 
	stTimerBase_Init.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
	stTimerBase_Init.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	stTimerBase_Init.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM2, &stTimerBase_Init);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&stTimerIc_Init);
	stTimerIc_Init.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &stTimerIc_Init);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM2,0);
	TIM_Cmd(TIM2, ENABLE); 
}
/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void encoder_init_tim4(void)
{
	TIM_TimeBaseInitTypeDef stTimerBase_Init;  
	TIM_ICInitTypeDef stTimerIc_Init;  
	GPIO_InitTypeDef stGpio_Init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟

	stGpio_Init.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置
	stGpio_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &stGpio_Init);					      //根据设定参数初始化GPIOB

	TIM_TimeBaseStructInit(&stTimerBase_Init);
	stTimerBase_Init.TIM_Prescaler = 0x0; // 预分频器 
	stTimerBase_Init.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
	stTimerBase_Init.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	stTimerBase_Init.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM4, &stTimerBase_Init);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&stTimerIc_Init);
	stTimerIc_Init.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &stTimerIc_Init);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4, ENABLE); 
}

/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int read_encoder(u8 TIMX)
{
	int Encoder_TIM;    
	switch(TIMX)
	{
		case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		default:  Encoder_TIM=0;
	}
	return Encoder_TIM;
}
/**************************************************************************
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}
/**************************************************************************
函数功能：TIM2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

