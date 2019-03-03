/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : timer.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : timer3 functions
  Function List :
              read_distance
              tim3_cap_init
              TIM3_IRQHandler
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#include "timer.h"

/**************************************************************************
函数功能：定时器3通道3输入捕获初始化
入口参数：入口参数：arr：自动重装值  psc：时钟预分频数 
返回  值：无
**************************************************************************/
TIM_ICInitTypeDef  stTim3Ic_Init;
void tim3_cap_init(u16 arr,u16 psc)	
{	 
	
	GPIO_InitTypeDef stGpio_Init;
	TIM_TimeBaseInitTypeDef  stTimerBase_Init;
 	NVIC_InitTypeDef stNvic_Init;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
	
	stGpio_Init.GPIO_Pin  = GPIO_Pin_0; 
	stGpio_Init.GPIO_Mode = GPIO_Mode_IPD; //PB0 输入  
	GPIO_Init(GPIOB, &stGpio_Init);
	
	stGpio_Init.GPIO_Pin  = GPIO_Pin_1;     
	stGpio_Init.GPIO_Mode = GPIO_Mode_Out_PP;     //PB1输出 
	stGpio_Init.GPIO_Speed = GPIO_Speed_2MHz;     //2M
	GPIO_Init(GPIOB, &stGpio_Init);
	
	//初始化定时器3 TIM3	 
	stTimerBase_Init.TIM_Period = arr; //设定计数器自动重装值 
	stTimerBase_Init.TIM_Prescaler =psc; 	//预分频器   
	stTimerBase_Init.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	stTimerBase_Init.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &stTimerBase_Init); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM3输入捕获参数
	stTim3Ic_Init.TIM_Channel = TIM_Channel_3; //CC1S=03 	选择输入端 IC3映射到TI1上
	stTim3Ic_Init.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	stTim3Ic_Init.TIM_ICSelection = TIM_ICSelection_DirectTI;
	stTim3Ic_Init.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	stTim3Ic_Init.TIM_ICFilter = 0x00;//配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &stTim3Ic_Init);
	
	//中断分组初始化
	stNvic_Init.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	stNvic_Init.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	stNvic_Init.NVIC_IRQChannelSubPriority = 2;  //从优先级0级
	stNvic_Init.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&stNvic_Init);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);//允许更新中断 ,允许CC3IE捕获中断	
	TIM_Cmd(TIM3,ENABLE ); 	//使能定时器3
}
/**************************************************************************
函数功能：超声波接收回波函数
入口参数：无
返回  值：无
**************************************************************************/
u16 TIM3CH3_CAPTURE_STA,TIM3CH3_CAPTURE_VAL;
void read_distance(void)
{   
	PBout(1)=1;
	delay_us(15);  
	PBout(1)=0;	
	if(TIM3CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
	{
		Distance=TIM3CH3_CAPTURE_STA&0X3F;
		Distance*=65536;					        //溢出时间总和
		Distance+=TIM3CH3_CAPTURE_VAL;		//得到总的高电平时间
		Distance=Distance*170/1000;
		//	printf("%d \r\n",Distance);
		TIM3CH3_CAPTURE_STA=0;			//开启下一次捕获
	}				
}
/**************************************************************************
函数功能：超声波回波脉宽读取中断
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void TIM3_IRQHandler(void)
{ 		    		  			    
	u16 tsr;
	tsr=TIM3->SR;
	if((TIM3CH3_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(tsr&0X01)//溢出
		{	    
			if(TIM3CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH3_CAPTURE_VAL=0XFFFF;
				}else 
					TIM3CH3_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x08)//捕获3发生捕获事件
		{	
			if(TIM3CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM3CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
				TIM3CH3_CAPTURE_VAL=TIM3->CCR3;	//获取当前的捕获值.
				TIM3->CCER&=~(1<<9);			//CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM3CH3_CAPTURE_STA=0;			//清空
				TIM3CH3_CAPTURE_VAL=0;
				TIM3CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM3->CNT=0;					//计数器清空
				TIM3->CCER|=1<<9; 				//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
	}
	TIM3->SR=0;//清除中断标志位 	     
}
