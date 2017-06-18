/******************************************************************************
  File			: standby.c
  Description	: wake up from standby mode test
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/5 21:00 PM	| Created
******************************************************************************/

#include "standby.h"
#include "led.h"
#include "delay.h"

/*-------------------------------------------------------------------
 * FUNC : wakeup_init
 * DESC : wakeup_init
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void wakeup_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	if(chech_wakeup() == 0)
		sys_standby();
}

/*-------------------------------------------------------------------
 * FUNC : sys_standby
 * DESC : sys_standby
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sys_standby(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_WakeUpPinCmd(ENABLE);
	PWR_EnterSTANDBYMode();	//进入待机模式
}

/*-------------------------------------------------------------------
 * FUNC : sys_enter_standby
 * DESC : sys_enter_standby
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sys_enter_standby(void)
{
	RCC_APB2PeriphResetCmd(0x01fc, DISABLE);
	sys_standby();
}

/*-------------------------------------------------------------------
 * FUNC : check_wakeup
 * DESC : check wake_up pin
 * PARM : res - 1: press until 3s and more
* 		   		0: error trig
 * RET	: N/A
 *-----------------------------------------------------------------*/
u8 chech_wakeup(void)
{
	u8 t = 0;	//记录按下的时间
	LED2 = 1;
	while(1){
		if(WAKEUP){
			t++;
			delay_ms(30);
			if(t >= 100){	//超过3s
				LED2 = 1;
				return 1;
			}
		}else{
			LED2 = 0;
			return 0;
		}
	}
}

void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	if(chech_wakeup())	//关机？
		sys_enter_standby();
}
