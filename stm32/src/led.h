#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define LED1	PDout(13)
#define LED2	PGout(14)

enum {
	OFF = 0x0,
	ON,
};

void led_init(void);

void led1_set(unsigned char on_off);

void led2_set(unsigned char on_off);

#endif /* _LED_H */
