#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

enum {
	OFF = 0x0,
	ON,
};

void led_init(void);

void led0_set(unsigned char on_off);

void led1_set(unsigned char on_off);

#endif /* _LED_H */
