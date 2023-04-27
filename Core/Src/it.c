/*
 * it.c
 *
 *  Created on: Apr 19, 2023
 *      Author: achar
 */

#include "system.h"

extern System my_sys;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == (&my_sys)->LED1_timer) {
		(&my_sys)->LEDToggle1 = 1; // flip flag
	}
	if (htim == (&my_sys)->LED2_timer) {
		(&my_sys)->LEDToggle2 = 1; // flip flag
	}
}


/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   Prevent unused argument(s) compilation warning
  UNUSED(huart);

   NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback can be implemented in the user file.

}
*/
