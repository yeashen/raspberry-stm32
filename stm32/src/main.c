/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/
#include "led.h"
#include "delay.h"
#include "motor.h"
#include "pwm.h"
#include "oled.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "sw_i2c.h"
#if USE_MPU6050
#if MPU6050_USE_DMP
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "hmc5883l.h"
#else
#include "mpu6050.h"
#endif
#endif

#include "encoder.h"
#include "user_stdlib.h"

#include "hw_config.h"
#include "usb_lib.h"

#if USE_MPU6050
#if MPU6050_USE_DMP
static signed char gyro_orientation[9] = {-1, 0, 0,
										   0,-1, 0,
										   0, 0, 1};

int result;
#endif
#endif

#define PWM_START	(630)

#if 0
int main(int argc, char *argv[])
{
	int ret = 0;
	Motor_PWM PWM_Ctrl;
	int pwm = PWM_START, dir = 0;
	char buf[20];

	delay_init();
	
	uart1_init(115200);

	uart2_init(115200);
	
	printf("init led...\r\n");
 	led_init();
	
	printf("init oled...\r\n");
	OLED_Init();
	OLED_ShowString(10,0, (u8 *)"RaspberrySTM32");
 	OLED_ShowString(0,16, (u8 *)"Li Xiaoming");  
	OLED_ShowString(0,32, (u8 *)"mail:");  
	OLED_ShowString(0,48, (u8 *)"2017/07/29"); 
	OLED_Refresh_Gram();

	printf("config NVIC...\r\n");
	NVIC_Configuration();
	
	printf("init timer3 pwm...\r\n");
	TIMER4_PWM_init(999, (uint16_t) (SystemCoreClock / 24000000) - 1);	//72MHz/(999+1)/(2+1)=24kHz
	
	printf("inti timer encoder...\r\n");
	Encoder_Init();
	
	printf("init motor...\r\n");
	motor_init();
	motor_direction_ctrl(LEFT, FOREWARD);
	motor_direction_ctrl(RIGHT, FOREWARD);
	
	printf("init timer1...\r\n");
	timer1_init(999, 71, 0);	//T = (999+1)*(71+1)/72MHz = 1ms
	
#if USE_MPU6050
	printf("init soft i2c...\r\n");
	#if defined(SW_I2C)
	SwI2C_init();
	#else 
	HwI2C_Init();
	#endif
	delay_ms(100);
#if 0	
	//MPU6050初始化
	if(mpu6050_check() != TRUE){
		printf("Check error! Please check!\r\n");
		return 0;
	}else{
		printf("Check OK.\r\n");
	}
#endif
	#if MPU6050_USE_DMP
	printf("init mpu6050...\r\n");
	ret = mpu_init(); 
	//printf("mpu init ret:%d \r\n", ret);

	if(!ret){	 		 
		printf("mpu initialization complete......\r\n");	 	  //mpu_set_sensor
		
		if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
			printf("mpu_set_sensor complete ......\r\n");
		else
			printf("mpu_set_sensor come across error ......\r\n");
		
		if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))	   	  //mpu_configure_fifo
			printf("mpu_configure_fifo complete ......\r\n");
		else
			printf("mpu_configure_fifo come across error ......\r\n");
		
		if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))	   	  //mpu_set_sample_rate
			printf("mpu_set_sample_rate complete ......\r\n");
		else
			printf("mpu_set_sample_rate error ......\r\n");
		
		if(!dmp_load_motion_driver_firmware())   	  //dmp_load_motion_driver_firmvare
			printf("dmp_load_motion_driver_firmware complete ......\r\n");
		else
			printf("dmp_load_motion_driver_firmware come across error ......\r\n");
		
		if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))) 	  //dmp_set_orientation
			printf("dmp_set_orientation complete ......\r\n");
		else
			printf("dmp_set_orientation come across error ......\r\n");
		
		if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
			DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
			DMP_FEATURE_GYRO_CAL))		   	  //dmp_enable_feature
			printf("dmp_enable_feature complete ......\r\n");
		else
			printf("dmp_enable_feature come across error ......\r\n");
		
		if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))   	  //dmp_set_fifo_rate
			printf("dmp_set_fifo_rate complete ......\r\n");
		else
			printf("dmp_set_fifo_rate come across error ......\r\n");
	  
		run_self_test();
		if(!mpu_set_dmp_state(1))
			printf("mpu_set_dmp_state complete ......\r\n");
		else
			printf("mpu_set_dmp_state come across error ......\r\n");
	}
	#else
	mpu6050_init();
	printf("mpu6050 init OK......\r\n");
	#endif
#endif

	printf("start timer1...\r\n");
	TIM_Cmd(TIM1, ENABLE);
	
	printf("system init OK\r\n");

	while(1)
	{
			
		#if 1 /* motor */
		if(dir){
			pwm -= 1;
			if(pwm < PWM_START)
				dir = 0;
		}else{
			pwm += 1;
			if(pwm > PWM_MAX_VALUE)
				dir = 1;
		}
		PWM_Ctrl.pwm1 = pwm;
		PWM_Ctrl.pwm2 = pwm;
		TIMER4_PWM_Refresh(&PWM_Ctrl);
		//printf("pwm=%d\r\n", pwm);
		delay_ms(50);
		#endif
		
		#if 0 /* led */
		led_rgb_set(LED_RED);
		delay_ms(100);
		led_rgb_set(LED_GREEN);
		delay_ms(100);
		led_rgb_set(LED_BLUE);
		delay_ms(100);
		#endif

		
		#if 0 /* printf */
		printf("hello stm32 for raspberry Pi\r\n");
		delay_ms(1000);
		#endif
	}

	return 0;
}
#endif

#if 1
extern u8 EP2BUSY;
int main(int argc, char *argv[])
{
	delay_init();
	
	uart1_init(115200);
	
	NVIC_Configuration();
	
	printf("usb hardward init...\r\n");

	printf("interrupt config\r\n");
	USB_Interrupts_Config();    

	printf("usb clk config\r\n");
	Set_USBClock();  

	printf("usb init\r\n");
	USB_Init();

	delay_ms(1000);

	printf("system initial OK.\r\n");
	
	while(1)
	{
		if(EP2BUSY==0)
		{
			EP2BUSY = 1;
			Joystick_Send(1,1);
			delay_ms(1000);
		}
	}
}
#endif
