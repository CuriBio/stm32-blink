/*
 * system.c
 *
 *  Created on: Apr 19, 2023
 *      Author: achar
 */

#include "system.h"

extern System my_sys;
void init_system(System *sys, TIM_HandleTypeDef *h_timer1, TIM_HandleTypeDef *h_timer2, GPIO_TypeDef *h_GPIO, uint16_t LEDPin1, uint16_t LEDPin2) {

	// fill my_sys with pointers and variables
	sys->h_timer1 = h_timer1;
	sys->h_timer2 = h_timer2;
	sys->h_GPIO_LED = h_GPIO;
	sys->LEDPin1 = LEDPin1;
	sys->LEDPin2 = LEDPin2;

	// initialize toggle variables
	sys->LEDToggle1 = 0;
	sys->LEDToggle2 = 0;

	// begin timers in interrupt mode
	HAL_TIM_Base_Start_IT(sys->h_timer1);
	HAL_TIM_Base_Start_IT(sys->h_timer2);

}




void system_main(System *sys) {
	while (1) {

		/* Loop functions */

		/*
		 * if (flag is raised)
		 * 		lower flag
		 * 		perform action
		 *
		 * or
		 *
		 * if (flag is raised)
		 * 		perform action, which also lowers flag
		 * */
		if (sys->LEDToggle1 == 1) { // check if flag is raised
			sys->LEDToggle1 = 0; // lower flag
			HAL_GPIO_TogglePin(sys->h_GPIO_LED, sys->LEDPin1); // toggle pin
		}
		if (sys->LEDToggle2 == 1) { // check if flag is raised
			sys->LEDToggle2 = 0; // lower flag
			HAL_GPIO_TogglePin(sys->h_GPIO_LED, sys->LEDPin2); // toggle pin
		}
	}
}

