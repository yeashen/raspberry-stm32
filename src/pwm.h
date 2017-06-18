#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
	 
void TIMER1_PWM_init(u16 arr, u16 psc);

#ifdef __cplusplus
}
#endif

#endif /* __PWM_H */
