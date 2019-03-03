/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : usart3.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : UART3 Function
  Function List :
              uart3_init
              USART3_IRQHandler
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#include "usart3.h"
#include <string.h> 

u8 u8Recv;

/**************************************************************************
函数功能：串口3初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void uart3_init(u32 bound)
{  	 
	//GPIO端口设置
	GPIO_InitTypeDef stGpio_Init;
	USART_InitTypeDef stUart_Init;
	NVIC_InitTypeDef stNvic_Init;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能UGPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	//USART3_TX  
	stGpio_Init.GPIO_Pin = GPIO_Pin_10; //PB.10
	stGpio_Init.GPIO_Speed = GPIO_Speed_50MHz;
	stGpio_Init.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &stGpio_Init);

	//USART3_RX	  
	stGpio_Init.GPIO_Pin = GPIO_Pin_11;//PB11
	stGpio_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &stGpio_Init);

	//Usart3 NVIC 配置
	stNvic_Init.NVIC_IRQChannel = USART3_IRQn;
	stNvic_Init.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	stNvic_Init.NVIC_IRQChannelSubPriority = 1;		//子优先级
	stNvic_Init.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&stNvic_Init);	//根据指定的参数初始化VIC寄存器
	//USART 初始化设置
	stUart_Init.USART_BaudRate = bound;//串口波特率
	stUart_Init.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	stUart_Init.USART_StopBits = USART_StopBits_1;//一个停止位
	stUart_Init.USART_Parity = USART_Parity_No;//无奇偶校验位
	stUart_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	stUart_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &stUart_Init);     //初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    //使能串口3 
}

/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	  
		s32 s32Recv_Tmp;
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
		s32Recv_Tmp=USART_ReceiveData(USART3); 
		u8Recv=s32Recv_Tmp;
	
		if(s32Recv_Tmp==0x59)  
			Flag_sudu=2;  //低速挡（默认值）
		if(s32Recv_Tmp==0x58)  
			Flag_sudu=1;  //高速档
		
		if(s32Recv_Tmp>10)  //默认使用
		{			
			if(s32Recv_Tmp==0x5A)	
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
			else if(s32Recv_Tmp==0x41)	
				Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////前
			else if(s32Recv_Tmp==0x45)	
				Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////后
			else if(s32Recv_Tmp==0x42||s32Recv_Tmp==0x43||s32Recv_Tmp==0x44)	
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  //左
			else if(s32Recv_Tmp==0x46||s32Recv_Tmp==0x47||s32Recv_Tmp==0x48)	    //右
				Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;
			else 
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
  		}
		if(s32Recv_Tmp<10)     //备用app为：MiniBalanceV1.0  因为MiniBalanceV1.0的遥控指令为A~H 其HEX都小于10
		{			
			Flag_sudu=1;//切换至高速档
			if(s32Recv_Tmp==0x00)
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
			else if(s32Recv_Tmp==0x01)	
				Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////前
			else if(s32Recv_Tmp==0x05)	
				Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////后
			else if(s32Recv_Tmp==0x02||s32Recv_Tmp==0x03||s32Recv_Tmp==0x04)	
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  //左
			else if(s32Recv_Tmp==0x06||s32Recv_Tmp==0x07||s32Recv_Tmp==0x08)	    //右
				Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;
			else 
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
		}	

		
		if(u8Recv==0x7B) 
			Flag_PID=1;   //APP参数指令起始位

		if(u8Recv==0x7D) 
			Flag_PID=2;   //APP参数指令停止位

		if(Flag_PID==1)  //采集数据
		{
			Receive[i]=u8Recv;
			i++;
		}
		if(Flag_PID==2)  //分析数据
		{
			if(Receive[3]==0x50) 	 
				PID_Send=1;
			else  if(Receive[3]==0x57) 	 
				Flash_Send=1;
			else  if(Receive[1]!=0x23) 
			{								
				for(j=i;j>=4;j--)
				{
					Data+=(Receive[j-1]-48)*pow(10,i-j);
				}
				switch(Receive[1])
				{
					case 0x30:  Balance_Kp=Data/100;break;
					case 0x31:  Balance_Kd=Data/100;break;
					case 0x32:  Velocity_Kp=Data/100;break;
					case 0x33:  Velocity_Ki=Data/100;break;
					case 0x34:  break; //预留
					case 0x35:  break; //预留
					case 0x36:  break; //预留
					case 0x37:  break; //预留
					case 0x38:  break; //预留
				}
			}				 
			Flag_PID=0;
			i=0;
			j=0;
			Data=0;
			memset(Receive, 0, sizeof(u8)*50);//数组清零
		} 
	}  											 
} 
