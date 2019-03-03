/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : timer.h
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : timer3 header
  Function List :
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H
#include <sys.h>	 
 
void tim3_cap_init(u16 arr,u16 psc);

void read_distance(void);

void TIM3_IRQHandler(void);
#endif
