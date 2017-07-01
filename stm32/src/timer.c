/******************************************************************************
  File			: timer.c
  Description	: timer test
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/5 21:00 PM	| Created
******************************************************************************/

#include "timer.h"
#include "led.h"
#include "delay.h"

static int time_count = 0;

/*-------------------------------------------------------------------
 * FUNC : timer2_init
 * DESC : timer 2 initial
 * PARM : arr - reload value
 * 		  psc - clock pre
 *		  cmd - 1: enable timer 0: don't enable timer
 * RET	: N/A
 *-----------------------------------------------------------------*/
void timer1_init(u16 arr, u16 psc, u8 cmd)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	if(cmd)
		TIM_Cmd(TIM1, ENABLE);
} 

void TIM1_IRQHandler(void)
{
	time_count++;
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){
		
		//10ms 定时任务
		if(time_count == 10){	
			time_count = 0;
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
