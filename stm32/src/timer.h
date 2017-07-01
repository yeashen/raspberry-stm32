#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
	 
void timer1_init(u16 arr, u16 psc, u8 cmd);
void timer1_cap_init(u16 arr, u16 psc);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H */
