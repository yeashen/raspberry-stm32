/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/

#include "led.h"
#include "delay.h"

int main(int argc, char *argv[])
{
	delay_init();
 	led_init();
	
	while(1)
	{
		led1_set(ON);
		led2_set(OFF);
		delay_ms(500);
		led1_set(OFF);
		led2_set(ON);
		delay_ms(500);
	}
}
