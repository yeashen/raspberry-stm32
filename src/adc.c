/******************************************************************************
  File			: adc.c
  Description	: adc test
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/8 21:00 PM	| Created
******************************************************************************/

#include "adc.h"
#include "DMA.h"
#include "delay.h"

vu16 AD_Value[N][ADC_CH_NUM]; //用来存放ADC转换结果，也是DMA的目标地址
vu16 After_filter[ADC_CH_NUM]; //用来存放求平均值之后的结果

vu16 mydata[ADC_CH_NUM];

/*-------------------------------------------------------------------
 * FUNC : adc_init
 * DESC : adc initial
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//分频 72M/6=12M
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA传输
	
	//PA.1.2.3作为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = ADC_CH_NUM;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
	// 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
	ADC_DMACmd(ADC1, ENABLE);
	
	ADC_Cmd(ADC1, ENABLE);//使能指定的ADC1
	ADC_ResetCalibration(ADC1);	//开启复位校准
	while(ADC_GetResetCalibrationStatus(ADC1));//获取ADC1复位校准寄存器的状态,设置状态则等待
	ADC_StartCalibration(ADC1); //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));

	DMA_Config(DMA1_Channel1, (u32)&ADC1->DR, /*(u32)&AD_Value*/(u32)mydata, ADC_CH_NUM);
	DMA_Enable(DMA1_Channel1);	//开启DMA
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


/*-------------------------------------------------------------------
 * FUNC : 
 * DESC : 
 * PARM : 
 * 		  
 * RET	: 
 *-----------------------------------------------------------------*/
void filter(void)
{
	int sum = 0;
	u8 i, j;
	for(i=0; i<ADC_CH_NUM; i++){
		for(j=0; j<N; j++){
			sum += AD_Value[j][i];
		}
		After_filter[i]=sum/N;
		sum=0;
	}
}

/*-------------------------------------------------------------------
 * FUNC : get_adc
 * DESC : get adc value
 * PARM : ch - convert which channel
 * RET	: res - convert adc value
 *-----------------------------------------------------------------*/
void get_adc(int16_t *ad_data)
{
	u8 i;
	filter();
	for(i=0; i<ADC_CH_NUM; i++)
		//ad_data[i] = After_filter[i] * (3300/4096);	//单位为mv
		ad_data[i] = mydata[i] * (3300/4096);
}
