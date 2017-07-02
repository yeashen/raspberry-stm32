#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "value_struct.h"

#define PWM_MAX_VALUE	999
	 
void TIMER4_PWM_init(u16 arr, u16 psc);

void TIMER4_PWM_Refresh(Motor_PWM *new_PWM_Value);

#ifdef __cplusplus
}
#endif

#endif /* __PWM_H */
