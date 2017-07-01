#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

enum {
	LEFT = 0x0,
	RIGHT,
};

enum {
	FOREWARD = 0x0,
	REVERSAL,
	STOP,
	BRAKE
};

void motor_init(void);

void motor_direction_ctrl(u8 side, u8 dir);

#endif /* _MOTOR_H */
