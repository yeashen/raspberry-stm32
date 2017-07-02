/******************************************************************************
  File			: PID_Ctrl.c
  Description	: PID Control algorithm
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/9/18 21:00 PM	| Created
******************************************************************************/

#include "PID_Ctrl.h"
#include "pwm.h"

PID pid;
Motor_PWM PWM_Ctrl;

void PID_init()
{
	pid.P = 1;
	pid.I = 0;
	pid.D = 0;
}

/*-------------------------------------------------------------------
 * FUNC : PID_Ctrl
 * DESC : PID control algorithm
 * PARM : new_PWM_Value - new PWM value
 * RET	: N/A
 *-----------------------------------------------------------------*/
void PID_Ctrl(Angle *m_angle, int16_t *m_gyro/*, int16_t thro*/)
{
	/*
	Angle angle;

	PWM_Ctrl.pwm1 = throttle + PID_pitch.out + PID_roll.out + PID_yaw.out;
	PWM_Ctrl.pwm2 = throttle - PID_pitch.out - PID_roll.out + PID_yaw.out;
	PWM_Ctrl.pwm3 = throttle - PID_pitch.out + PID_roll.out - PID_yaw.out;
	PWM_Ctrl.pwm4 = throttle + PID_pitch.out - PID_roll.out - PID_yaw.out;
	*/
	TIMER4_PWM_Refresh(&PWM_Ctrl);
}
