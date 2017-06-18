#ifndef __STANDBY_H
#define __STANDBY_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "sys.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_exti.h"

#define WAKEUP	PAin(0)

void wakeup_init(void);	 
void sys_standby(void);
void sys_enter_standby(void);
u8 chech_wakeup(void);

#ifdef __cplusplus
}
#endif

#endif /* __STANDBY_H */
