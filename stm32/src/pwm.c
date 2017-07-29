/******************************************************************************
  File			: pwm.c
  Description	: pwm test
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/4 21:00 PM	| Created
******************************************************************************/

#include "pwm.h"

/*-------------------------------------------------------------------
 * FUNC : TIMER4_PWM_init
 * DESC : timer4 pwm initial
 * PARM : arr - reload value
 *		  psc - clock pre
 * RET	: N/A
 *-----------------------------------------------------------------*/
void TIMER4_PWM_init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, DISABLE);
	
	//PB 6.7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	//TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	
	/* advance timer setting */
	//TIM_CtrlPWMOutputs(TIM4, ENABLE);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

/*-------------------------------------------------------------------
 * FUNC : TIMER3_PWM_Refresh
 * DESC : timer1 pwm value refresh
 * PARM : new_PWM_Value - new PWM value
 * RET	: N/A
 *-----------------------------------------------------------------*/
void TIMER4_PWM_Refresh(Motor_PWM *new_PWM_Value)
{
	Motor_PWM PWM_Value;
	PWM_Value.pwm1 = new_PWM_Value->pwm1;
	PWM_Value.pwm2 = new_PWM_Value->pwm2;
	if(new_PWM_Value->pwm1 > PWM_MAX_VALUE) PWM_Value.pwm1 = PWM_MAX_VALUE;
	if(new_PWM_Value->pwm2 > PWM_MAX_VALUE) PWM_Value.pwm2 = PWM_MAX_VALUE;
	if(new_PWM_Value->pwm1 < 0)	PWM_Value.pwm1 = 0;
	if(new_PWM_Value->pwm2 < 0)	PWM_Value.pwm2 = 0;

	TIM_SetCompare1(TIM4, PWM_Value.pwm1);
	TIM_SetCompare2(TIM4, PWM_Value.pwm2);
}
