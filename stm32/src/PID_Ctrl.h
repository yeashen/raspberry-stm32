#ifndef __PID_CTRL_H
#define __PID_CTRL_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "pwm.h"
#include "value_struct.h"

#define EXPECT	0

void PID_init(void);
void PID_Ctrl(Angle *m_angle, int16_t *m_gyro);	 

#ifdef __cplusplus
}
#endif

#endif /* __PID_CTRL_H */

