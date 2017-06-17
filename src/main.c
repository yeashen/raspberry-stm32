/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/

#include "led.h"

int main(int argc, char *argv[])
{
	int delay;
	
 	led_init();
	
	while(1)
	{
		led0_set(ON);
		led1_set(ON);
		delay=500000;
		while(delay)
			delay--;
		led0_set(OFF);
		led1_set(OFF);
		delay=500000;
		while(delay)
			delay--;
	}
}
