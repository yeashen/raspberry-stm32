#ifndef __VALUE_STRUCT_H
#define __VALUE_STRUCT_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"

typedef enum{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
}AXIS;

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
}Axis3i16;

typedef struct
{
	int32_t x;
	int32_t y;
	int32_t z;
}Axis3i32;

typedef struct
{
	float x;
	float y;
	float z;
}Axis3f;


typedef struct
{
	Axis3i16 m_accel;
	Axis3i16 m_gyro;
	//int16_t m_temp;
}MPUData;

typedef struct
{
	Axis3f c_accel;
	Axis3f c_gyro;
	float c_temp;
	Axis3f c_accel_offset;
	Axis3f c_gyro_offset;
}MPUCovData;

typedef struct
{
	Axis3i16 mag;
}MAG_DATA;

//For Motor PWM output
typedef struct
{
	int16_t pwm1;
	int16_t pwm2;
}Motor_PWM;

//For PID Control
typedef struct
{
	float P;
	float I;
	float D;
	float Pout;
	float Iout;
	float Dout;
	float out;
	float Max;
}PID;

typedef struct
{
	float pitch;
	float roll;
	float yaw;
}Angle;

#ifdef __cplusplus
}
#endif

#endif /* __VALUE_STRUCT_H */

