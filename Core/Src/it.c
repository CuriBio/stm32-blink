/*
 * it.c
 *
 *  Created on: Apr 19, 2023
 *      Author: achar
 */

#include "system.h"

extern System my_sys;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == (&my_sys)->h_timer1) {
		(&my_sys)->LEDToggle1 = 1; // flip flag
	}
	if (htim == (&my_sys)->h_timer2) {
		(&my_sys)->LEDToggle2 = 1; // flip flag
	}
}
