/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : encoder.h
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : encoder speed
  Function List :
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。

void encoder_init_tim2(void);

void encoder_init_tim4(void);

int read_encoder(u8 TIMX);

void TIM4_IRQHandler(void);

void TIM2_IRQHandler(void);

#endif
