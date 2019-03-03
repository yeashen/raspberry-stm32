/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : usart3.h
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : uart3 header
  Function List :
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#ifndef __USRAT3_H
#define __USRAT3_H 
#include "sys.h"	  	

void uart3_init(u32 bound);

void USART3_IRQHandler(void);

#endif

