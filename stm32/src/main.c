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
#include "sys.h"

int main(int argc, char *argv[])
{
	Motor_PWM PWM_Ctrl;
	int pwm = 500, dir = 0;
	
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
		#if 1 /* motor */
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
		//delay_ms(5000);
	}
}
