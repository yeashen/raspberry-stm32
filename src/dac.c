/******************************************************************************
  File			: dac.c
  Description	: dac test
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/8 21:00 PM	| Created
******************************************************************************/

#include "dac.h"

/*-------------------------------------------------------------------
 * FUNC : dac1_init
 * DESC : dac1 initial
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void dac1_init(void)
{
	DAC_InitTypeDef DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	//PA.4作为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}

/*-------------------------------------------------------------------
 * FUNC : dac1_set_vol
 * DESC : set dac channel1 output voltage
 * PARM : vol - 0~3300 means 0~3.3V
 * RET	: N/A
 *-----------------------------------------------------------------*/
void dac1_set_vol(u16 vol)
{
	float temp = vol;
	temp /= 1000;
	temp = temp * 4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R, temp);
}
