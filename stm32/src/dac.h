#ifndef __DAC_H
#define __DAC_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dac.h"
	 
void dac1_init(void);
void dac1_set_vol(u16 vol);

#ifdef __cplusplus
}
#endif

#endif /* __DAC_H */
