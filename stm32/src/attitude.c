/******************************************************************************
  File			: attitude.c
  Description	: quadrocopter attitude calculate algorithm
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/10/9 21:00 PM	| Created
******************************************************************************/

#include "attitude.h"

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;	//初始化四元数
float exInt = 0, eyInt = 0, ezInt = 0;	//姿态解算误差积分

void data_filter(MPUData *data_in, MPUData *data_out)
{
	static uint8_t 	filter_cnt=0;
	static int16_t	ACC_X_BUF[FILTER_NUM],ACC_Y_BUF[FILTER_NUM],ACC_Z_BUF[FILTER_NUM];
	int32_t temp1=0,temp2=0,temp3=0;
	uint8_t i;

	ACC_X_BUF[filter_cnt] = data_in->m_accel.x;
	ACC_Y_BUF[filter_cnt] = data_in->m_accel.y;
	ACC_Z_BUF[filter_cnt] = data_in->m_accel.z;
	for(i=0;i<FILTER_NUM;i++)
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
	}
	data_out->m_accel.x = temp1 / FILTER_NUM;
	data_out->m_accel.y = temp2 / FILTER_NUM;
	data_out->m_accel.z = temp3 / FILTER_NUM;
	filter_cnt++;
	if(filter_cnt==FILTER_NUM)	filter_cnt=0;
}


/*-------------------------------------------------------------------
 * FUNC : IMUupdate
 * DESC : 四轴姿态解算，借助mpu6050得到的四元数，求的欧拉角
 *	      由于加速度的噪声大，此处的数据为采用滤波后的数据
 * PARM : mpu_data - mpu6050读的的角速度和加速度
 		  angle - 求得的欧拉角
 * RET	: N/A
 *-----------------------------------------------------------------*/
void IMUupdate(MPUData *mpu_data, Angle *angle)
{
	float ax, ay, az, gx, gy, gz;
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;

	ax = mpu_data->m_accel.x;
	ay = mpu_data->m_accel.y;
	az = mpu_data->m_accel.z;
	gx = mpu_data->m_gyro.x;
	gy = mpu_data->m_gyro.y;
	gz = mpu_data->m_gyro.z;

	//原始数据归一化，得到单位加速度
	norm = sqrt(ax*ax + ay*ay + az*az);
	ax /= norm;
	ay /= norm;
	az /= norm;
	//把四元数换算成“方向余弦矩阵”中的第三列的三个元素
	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

	ex = ay*vz - az*vy;
	ey = az*vx - ax*vz;
	ez = ax*vy - ay*vx;
	exInt += ex*ki;
	eyInt += ey*ki;
	ezInt += ez*ki;

	//用叉乘误差来做PI修正陀螺零偏，通过调节Kp，Ki两个参数，
	//可以控制加速度计修正陀螺仪积分姿态的速度
	gx += (kp*ex + exInt);
	gy += (kp*ey + eyInt);
	gz += (kp*ez + ezInt);

	q0 += (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 += (q0*gx + q2*gz - q3*gy)*halfT;
	q2 += (q0*gy - q1*gz + q3*gx)*halfT;
	q3 += (q0*gz + q1*gy - q2*gx)*halfT;

	//四元数单位化
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 /= norm;
	q1 /= norm;
	q2 /= norm;
	q3 /= norm;

	angle->pitch = asin(2 * q1 * q3 - 2 * q0* q2)* 57.3; // pitch
	angle->roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	//angle.yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	angle->yaw += mpu_data->m_gyro.z*Gyro_G*0.002f;
}
