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
#include "mpu6050.h"
#include "sys.h"

int main(int argc, char *argv[])
{
	//Motor_PWM PWM_Ctrl;
	//int pwm = 500, dir = 0;
	
	delay_init();
 	//led_init();
	NVIC_Configuration();
	TIMER4_PWM_init(999, (uint16_t) (SystemCoreClock / 24000000) - 1);	//72MHz/(999+1)/(2+1)=24kHz
	motor_init();
	motor_direction_ctrl(LEFT, FOREWARD);
	motor_direction_ctrl(RIGHT, FOREWARD);
	
	while(1)
	{
		#if 0 /* led */
		led_set(LED_RED);
		delay_ms(500);
		led_set(LED_GREEN);
		delay_ms(500);
		led_set(LED_BLUE);
		delay_ms(500);
		#endif
		
		#if 0 /* motor */
		if(dir){
			pwm -= 50;
			if(pwm < 500)
				dir = 0;
		}else{
			pwm += 50;
			if(pwm > PWM_MAX_VALUE)
				dir = 1;
		}
		PWM_Ctrl.pwm1 = pwm;
		PWM_Ctrl.pwm2 = pwm;
		TIMER4_PWM_Refresh(&PWM_Ctrl);
		delay_ms(500);
		#endif
		
		#if 1 /* mpu6050 */
		#ifdef MPU6050_USE_DMP
		result = mpu_init(); 
		//printf("mpu init result:%d \r\n", result);

		if(!result){	 		 
			//printf("mpu initialization complete......\r\n");	 	  //mpu_set_sensor
			
			if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
				//printf("mpu_set_sensor complete ......\r\n");
			else
				//printf("mpu_set_sensor come across error ......\r\n");
			
			if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))	   	  //mpu_configure_fifo
				//printf("mpu_configure_fifo complete ......\r\n");
			else
				//printf("mpu_configure_fifo come across error ......\r\n");
			
			if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))	   	  //mpu_set_sample_rate
				//printf("mpu_set_sample_rate complete ......\r\n");
			else
				//printf("mpu_set_sample_rate error ......\r\n");
			
			if(!dmp_load_motion_driver_firmware())   	  //dmp_load_motion_driver_firmvare
				//printf("dmp_load_motion_driver_firmware complete ......\r\n");
			else
				//printf("dmp_load_motion_driver_firmware come across error ......\r\n");
			
			if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))) 	  //dmp_set_orientation
				//printf("dmp_set_orientation complete ......\r\n");
			else
				//printf("dmp_set_orientation come across error ......\r\n");
			
			if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
				DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
				DMP_FEATURE_GYRO_CAL))		   	  //dmp_enable_feature
				//printf("dmp_enable_feature complete ......\r\n");
			else
				//printf("dmp_enable_feature come across error ......\r\n");
			
			if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))   	  //dmp_set_fifo_rate
				//printf("dmp_set_fifo_rate complete ......\r\n");
			else
				//printf("dmp_set_fifo_rate come across error ......\r\n");
		  
			run_self_test();
			if(!mpu_set_dmp_state(1))
				//printf("mpu_set_dmp_state complete ......\r\n");
			else
				//printf("mpu_set_dmp_state come across error ......\r\n");
		}
	#else
		mpu6050_init();
		//printf("mpu6050 init OK......\r\n");
	#endif
	#endif
	}
}
