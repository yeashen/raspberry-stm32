/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : motor.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : motor funcitons
  Function List :
              motor_gpio_init
              robot_pwm_init
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#include "motor.h"

static void motor_gpio_init(void)
{
	GPIO_InitTypeDef stGpio_Init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
	stGpio_Init.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//端口配置
	stGpio_Init.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	stGpio_Init.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOB, &stGpio_Init);					      //根据设定参数初始化GPIOB 
}
void robot_pwm_init(u16 arr,u16 psc)
{		 		
	GPIO_InitTypeDef stGpio_Init;
	TIM_TimeBaseInitTypeDef  stTimerBase_Init;
	TIM_OCInitTypeDef  stTimerOc_Init;
	
	motor_gpio_init();	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
   //设置该引脚为复用输出功能,输出TIM1 CH1 CH4的PWM脉冲波形
	stGpio_Init.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM_CH1 //TIM_CH4
	stGpio_Init.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	stGpio_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &stGpio_Init);
	
	stTimerBase_Init.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	stTimerBase_Init.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	stTimerBase_Init.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	stTimerBase_Init.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &stTimerBase_Init); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	stTimerOc_Init.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	stTimerOc_Init.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	stTimerOc_Init.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	stTimerOc_Init.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &stTimerOc_Init);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM1, &stTimerOc_Init);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4预装载使能	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
 
} 

