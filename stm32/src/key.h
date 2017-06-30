#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define KEY0	GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)
#define KEY1	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)

#define KEY0_PRESS	1
#define KEY1_PRESS	2	

#define LEFT		10
#define RIGHT		11
#define UP			12
#define DOWN		13
#define POWER_UP	14
#define POWER_OFF	15
#define NO_PRESS	255

#define KEY_PDEBUG
#ifdef KEY_PDEBUG
#define KEY_DEBUG(string,args...)	printf("[MKEY_DEBUG] ");	\
									printf(string, ##args);
#else
#define KEY_DEBUG(string,args...)
#endif	
	 
void key_init(void);
u8 key_scan(void);
void matrix_key_init(void);
u8 matrix_key_scan(void);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
