/*
 * system.c
 *
 *  Created on: Apr 19, 2023
 *      Author: achar
 */

#include "system.h"


extern System my_sys;
void init_system(System *sys,
		TIM_HandleTypeDef *h_timer1, TIM_HandleTypeDef *h_timer2,
		GPIO_TypeDef *h_GPIO,
		uint16_t LEDPin1, uint16_t LEDPin2 ,
		uint16_t size_of_recieve_buffer) {

	// fill my_sys with pointers and variables
	sys->h_timer1 = h_timer1;
	sys->h_timer2 = h_timer2;
	sys->h_GPIO_LED = h_GPIO;
	sys->LEDPin1 = LEDPin1;
	sys->LEDPin2 = LEDPin2;

	// initialize toggle variables
	sys->LEDToggle1 = 0;
	sys->LEDToggle2 = 0;

	sys->LED1_timer = h_timer1;
	sys->LED2_timer = h_timer2;

	// begin timers in interrupt mode
	HAL_TIM_Base_Start_IT(sys->LED1_timer);
	HAL_TIM_Base_Start_IT(sys->LED2_timer);

	sys->recieve_ring_buffer = ring_buffer_init(size_of_recieve_buffer /*, Thread_Code_t this_thread_code*/);

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

		uint8_t incomingByte;
		HAL_StatusTypeDef status = HAL_UART_Receive_IT(&huart3, &incomingByte, SIZE_INCOMING_DATA);
		if (!status) {
			sys->recieve_ring_buffer->push(sys->recieve_ring_buffer, incomingByte);
		}

		commandHandler(&my_sys);


	}
}


void commandHandler(System *sys) {
	uint8_t command;
	uint8_t status_error = pop_ring_buffer(sys->recieve_ring_buffer, &command);
	if (!status_error) {
		switch (command) {
		case LED1_5Hz:
			sys->LED1_timer = sys->h_timer1;
			break;
		case LED1_1Hz:
			sys->LED1_timer = sys->h_timer2;
			break;
		case LED2_5Hz:
			sys->LED2_timer = sys->h_timer1;
			break;
		case LED2_1Hz:
			sys->LED2_timer = sys->h_timer2;
			break;
		}
	}

}
