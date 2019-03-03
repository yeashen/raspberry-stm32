/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : key.h
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/26
  Last Modified :
  Description   : key funcitons
  Function List :
  History       :
  1.Date        : 2019/2/26
    Author      : Juven
    Modification: Created file

******************************************************************************/

#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY PAin(5)

void key_init(void);          //按键初始化

u8 click_n_double(u8 u8Time);  //单击按键扫描和双击按键扫描

u8 click(void);               //单击按键扫描

u8 long_press(void);           //长按扫描  

#endif  
