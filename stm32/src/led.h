#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

enum {
	LED_RED = 0x0,
	LED_GREEN,
	LED_BLUE,
};

void led_init(void);

void led_set(u8 rgb);

#endif /* _LED_H */
