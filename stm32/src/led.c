#include "led.h"

/**************************************************************************
函数功能：LED接口初始化
入口参数：无 
返回  值：无
**************************************************************************/
void led_init(void)
{
	GPIO_InitTypeDef stGpio_Init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能端口时钟
	stGpio_Init.GPIO_Pin = GPIO_Pin_4;	            //端口配置
	stGpio_Init.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	stGpio_Init.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOA, &stGpio_Init);				   //根据设定参数初始化GPIOA 
}

/**************************************************************************
函数功能：LED闪烁
入口参数：闪烁频率 
返回  值：无
**************************************************************************/
void led_flash(u16 u16Time)
{
	static s32 S32tTemp;
	if(0 == u16Time) 
		LED=0;
	else if(++S32tTemp == u16Time)	
		LED=~LED,S32tTemp=0;
}
