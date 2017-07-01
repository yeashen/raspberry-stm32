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

int main(int argc, char *argv[])
{
	delay_init();
 	//led_init();
	//TIMER4_PWM_init(999, 141);
	motor_init();
	
	//motor_direction_ctrl(LEFT, BRAKE);
	//motor_direction_ctrl(RIGHT, BRAKE);
	
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
		motor_direction_ctrl(LEFT, FOREWARD);
		motor_direction_ctrl(RIGHT, FOREWARD);
		delay_ms(5000);
		motor_direction_ctrl(LEFT, REVERSAL);
		motor_direction_ctrl(RIGHT, REVERSAL);
		delay_ms(5000);
		#endif
	}
}
