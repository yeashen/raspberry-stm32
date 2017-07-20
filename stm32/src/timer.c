/******************************************************************************
  File			: timer.c
  Description	: timer test
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/5 21:00 PM	| Created
******************************************************************************/

#include "timer.h"
#include "led.h"
#include "delay.h"
#if USE_MPU6050
#if MPU6050_USE_DMP
#include "hmc5883l.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#else
#include "mpu6050.h"
#endif
#endif
#include "oled.h"
#include "value_struct.h"
#include "attitude.h"

static int task1_count = 0;
static int task2_count = 0;
static int led_sta = 0;

#if USE_MPU6050
#if defined(GET_XYZ)
MPUData m_data;	//原始数据
MPUData f_data;	//滤波后的数据
Angle quat_angle;
int16_t m_pitch, m_roll, m_yaw;
#elif defined(GET_Z)
int16_t acc_z;
int16_t rot_z;
float gyro_z;
float angle_z;
int16_t zero_acc = 0;
int16_t zero_gyro = 0;
#endif

#define q30  1073741824.0f

#if MPU6050_USE_DMP
int16_t gyro[3], accel[3];
unsigned long sensor_timestamp;
unsigned long sensor_timestamp;
short sensors;
unsigned char more;
long quat[4];
#endif
#endif

/*-------------------------------------------------------------------
 * FUNC : timer2_init
 * DESC : timer 2 initial
 * PARM : arr - reload value
 * 		  psc - clock pre
 *		  cmd - 1: enable timer 0: don't enable timer
 * RET	: N/A
 *-----------------------------------------------------------------*/
void timer1_init(u16 arr, u16 psc, u8 cmd)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	if(cmd)
		TIM_Cmd(TIM1, ENABLE);
} 
#if USE_MPU6050
void data_handle()
{
#if MPU6050_USE_DMP
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	if(sensors & INV_WXYZ_QUAT ){
		q0 = quat[0] / q30;	
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30;
		quat_angle.pitch = asin(2 * q1 * q3 - 2 * q0* q2)* 57.3; // pitch
		quat_angle.roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
		quat_angle.yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
		m_pitch = quat_angle.pitch * 100;
		m_roll = quat_angle.roll * 100;
		m_yaw = quat_angle.yaw * 10;
	}
#else
#if defined(GET_XYZ)
	m_pitch = quat_angle.pitch * 100;
	m_roll = quat_angle.roll * 100;
	m_yaw = quat_angle.yaw * 10;
#elif defined(GET_Z)
	/* acc range 2g, rate 16384 LSB/g */
	angle_z = (acc_z - zero_acc) / 16384;
	angle_z = angle_z*1.2*180/3.14;
	/* gyro rane 500deg/s reate 65.5 LSB/(deg/s) */
	gyro_z = (rot_z - zero_gyro)/65.5;
#endif
#endif
}

void data_display()
{
#if 0
	char buf[128];

	sprintf(buf, "%d", m_yaw);
	OLED_ShowString(20,16, (u8 *)buf); 
	sprintf(buf, "%d", m_roll);	
	OLED_ShowString(20,32, (u8 *)buf);  
	sprintf(buf, "%d", m_pitch);
	OLED_ShowString(20,48, (u8 *)buf); 
	OLED_Refresh_Gram();
#endif
}
#endif

void TIM1_UP_IRQHandler(void)
{
	task1_count++;
	task2_count++;
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){
		
		//10ms 定时任务
		if(task1_count == 10){	
			#if USE_MPU6050
			#if MPU6050_USE_DMP
			dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);
			#else
			#if defined(GET_XYZ)
			mpu6050_getMotion6(&m_data.m_accel.x, &m_data.m_accel.y, &m_data.m_accel.z, \
				&m_data.m_gyro.x, &m_data.m_gyro.y, &m_data.m_gyro.z);
			data_filter(&m_data, &f_data);
			IMUupdate(&f_data, &quat_angle);
			#elif defined(GET_Z)
			acc_z =  mpu6050_getAccelerationZ();
			rot_z =  mpu6050_getRotationZ();
			#endif
			#endif

			data_handle();
			
			//data_display();
			#endif
			
			task1_count = 0;
		}
		//100ms 定时任务
		if(task2_count == 100){
			if(led_sta){
				led_set(LED_ON);
				led_sta = 0;
			}else{
				led_set(LED_OFF);
				led_sta = 1;
			}
			printf("acc=%d, rot=%d, angle=%d, gyro=%d\r\n", acc_z, rot_z, angle_z, gyro_z);	
			task2_count = 0;
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
#if 0
void timer2_encoder_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    //PA0: ch1  PA1: ch2  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM2 | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                           


    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period = 359*4;  //设定计数器重装值   TIMx_ARR = 359*4
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3时钟预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
    TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
    TIM_ICInitStructure.TIM_ICFilter = 6;  //选择输入比较滤波器 
    TIM_ICInit(TIM4, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM3

//  TIM_ARRPreloadConfig(TIM4, ENABLE);//使能预装载
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM3的更新标志位
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//运行更新中断
    //Reset counter
    TIM4->CNT = 0;//

    TIM_Cmd(TIM4, ENABLE);   //启动TIM4定时器

}
#endif
