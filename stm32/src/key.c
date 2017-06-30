/******************************************************************************
  File			: key.c
  Description	: key test
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/4 21:00 PM	| Created
******************************************************************************/

#include "key.h"
#include "delay.h"
#include "usart.h"

struct io_port
{
	GPIO_TypeDef *GPIO_x;
	uint16_t GPIO_pin;
};

static struct io_port key_input[4] = 
{
	{GPIOD, GPIO_Pin_0},
	{GPIOD, GPIO_Pin_1},
	{GPIOD, GPIO_Pin_2},
	{GPIOD, GPIO_Pin_3}
};

static struct io_port key_output[4] = 
{
	{GPIOD, GPIO_Pin_4},
	{GPIOD, GPIO_Pin_5},
	{GPIOD, GPIO_Pin_6},
	{GPIOD, GPIO_Pin_7}
};

u8 key_table[4][4] = 
{
	{LEFT,1,4,7},
	{0,2,5,8},
	{RIGHT,3,6,9},
	{DOWN,UP,POWER_OFF,POWER_UP}
};

/*-------------------------------------------------------------------
 * FUNC : key_init
 * DESC : key initial
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*-------------------------------------------------------------------
 * FUNC : key_scan
 * DESC : key scan press
 * PARM : N/A
 * RET	: res - press key value 
 *-----------------------------------------------------------------*/
u8 key_scan(void)
{
	if(KEY0 == 0||KEY1 == 0){
		delay_ms(10);
		if(KEY0 == 0){
			while(!KEY0);
			return KEY0_PRESS;
		}
		else if(KEY1 == 0){
			while(!KEY1);
			return KEY1_PRESS;
		}
	}
	return 0;
}

/*-------------------------------------------------------------------
 * FUNC : matrix_key_init
 * DESC : matrix key initail, 4x4 key
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void matrix_key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	u8 i;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	//PD.0-PD.3 配置为输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//PD.4-PD.7 配置是输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	for(i = 0; i < 4; i++){
		GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
	}
}

/*-------------------------------------------------------------------
 * FUNC : matrix_key_scan
 * DESC : matrix key scan, 4x4 key
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
u8 matrix_key_scan(void)
{
	u8 i, j, key_value = NO_PRESS;
	for(i = 0; i < 4; i++){		//依次置低列电平
		GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
		for(j = 0; j < 4; j++){
			if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0){
				delay_ms(10);	//延时消抖
				if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0){
					key_value = key_table[i][j];
					KEY_DEBUG("press key: %d\r\n", key_value);
					while(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0);
					break;
				}
			}
		}
		GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
	}
	return key_value;
}
