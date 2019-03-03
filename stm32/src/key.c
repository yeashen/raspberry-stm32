/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : key.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : key functions 
  Function List :
              click
              click_n_double
              key_init
              long_press
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#include "key.h"
/**************************************************************************
函数功能：按键初始化
入口参数：无
返回  值：无 
**************************************************************************/
void key_init(void)
{
	GPIO_InitTypeDef stGpio_Init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PA端口时钟
	stGpio_Init.GPIO_Pin = GPIO_Pin_5;	            //端口配置
	stGpio_Init.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
	GPIO_Init(GPIOA, &stGpio_Init);				  //根据设定参数初始化GPIOA 
} 
/**************************************************************************
函数功能：按键扫描
入口参数：双击等待时间
返回  值：按键状态 0：无动作 1：单击 2：双击 
**************************************************************************/
u8 click_n_double (u8 u8Time)
{
	static	u8 flag_key,count_key,double_key;	
	static	u16 count_single,Forever_count;
	if(KEY==0)  
		Forever_count++;   //长按标志位未置1
	else        
	 	Forever_count=0;
	if(0==KEY&&0==flag_key)		
		flag_key=1;	
	
	if(0==count_key)
	{
		if(flag_key==1) 
		{
			double_key++;
			count_key=1;	
		}
		if(double_key==2) 
		{
			double_key=0;
			count_single=0;
			return 2;//双击执行的指令
		}
	}
	if(1==KEY)			
		flag_key=0,count_key=0;
	
	if(1==double_key)
	{
		count_single++;
		if(count_single>u8Time&&Forever_count<u8Time)
		{
			double_key=0;
			count_single=0;	
			return 1;//单击执行的指令
		}
		if(Forever_count>u8Time)
		{
			double_key=0;
			count_single=0;	
		}
	}	
	return 0;
}
/**************************************************************************
函数功能：按键扫描
入口参数：无
返回  值：按键状态 0：无动作 1：单击 
**************************************************************************/
u8 click(void)
{
	static u8 flag_key=1;//按键按松开标志
	if(flag_key&&KEY==0)
	{
		flag_key=0;
		return 1;	// 按键按下
	}
	else if(1==KEY)			
		flag_key=1;
	
	return 0;//无按键按下
}
/**************************************************************************
函数功能：长按检测
入口参数：无
返回  值：按键状态 0：无动作 1：长按2s
**************************************************************************/
u8 long_press(void)
{
	static u16 Long_Press_count,long_press;
	if(long_press==0&&KEY==0)  
		Long_Press_count++;   //长按标志位未置1
	else                       
		Long_Press_count=0; 
	
	if(Long_Press_count>200)		
	{
		long_press=1;	
		Long_Press_count=0;
		return 1;
	}				
	if(long_press==1)     //长按标志位置1
	{
		long_press=0;
	}
	return 0;
}
