/*
 * system.c
 *
 *  Created on: Apr 19, 2023
 *      Author: achar
 */

#include "system.h"
#include "stdlib.h"

//extern System my_sys;
extern UART_HandleTypeDef huart3;


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;


void init_system(System *sys,
		GPIO_TypeDef *h_GPIO,
		uint16_t LEDPin1, uint16_t LEDPin2 ,
		uint16_t size_of_receive_buffer) {

	// fill my_sys with pointers and variables
	sys->h_timer1 = &htim2;
	sys->h_timer2 = &htim3;
	sys->h_GPIO_LED = h_GPIO;
	sys->LEDPin1 = LEDPin1;
	sys->LEDPin2 = LEDPin2;

	// initialize toggle variables
	sys->LEDToggle1 = 0;
	sys->LEDToggle2 = 0;
	sys->begin_LED1 = FALSE;
	sys->begin_LED2 = FALSE;

	sys->LED1_timer = sys->h_timer1;
	sys->LED2_timer = sys->h_timer2;

	// ring buffer and UART variables
	sys->incomingByte = (uint8_t*)malloc(sizeof(uint8_t*));
	sys->receive_ring_buffer = ring_buffer_init(size_of_receive_buffer /*, Thread_Code_t this_thread_code*/);

	sys->received_input = FALSE;
	sys->handshakeTimer = &htim4;
	// begin interrupts
	HAL_TIM_Base_Start_IT(sys->LED1_timer);
	HAL_TIM_Base_Start_IT(sys->LED2_timer);
	HAL_TIM_Base_Start_IT(sys->handshakeTimer);

	HAL_UART_Receive_IT(&huart3, sys->incomingByte, SIZE_INCOMING_DATA); // begin UART IT
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
		if (sys->LEDToggle1 == 1 && sys->begin_LED1) { // check if flag is raised
			sys->LEDToggle1 = 0; // lower flag
			HAL_GPIO_TogglePin(sys->h_GPIO_LED, sys->LEDPin1); // toggle pin
		}
		if (sys->LEDToggle2 == 1 && sys->begin_LED2) { // check if flag is raised
			sys->LEDToggle2 = 0; // lower flag
			HAL_GPIO_TogglePin(sys->h_GPIO_LED, sys->LEDPin2); // toggle pin
		}

		if (!sys->receive_ring_buffer->is_empty(sys->receive_ring_buffer)) { // there is a command in the ring buffer
			commandHandler(sys); // handle any commands in the ring buffer
			// actions performed in commandHandler causes ring buffer to empty
		}





	}
}


void commandHandler(System *sys) {
	uint8_t command;
	uint8_t status_error = pop_ring_buffer(sys->receive_ring_buffer, &command);
	if (!status_error) {
		switch (command) {
		case LED1_5Hz:
			sys->LED1_timer = sys->h_timer1; // use h_timer1 as the LED1 timer
			if (sys->begin_LED1) {
				HAL_GPIO_WritePin(sys->h_GPIO_LED, sys->LEDPin1, GPIO_PIN_RESET); // toggle pin
				sys->begin_LED1 = FALSE;
			}
			else {
				sys->begin_LED1 = TRUE;
			}
			break;
		case LED1_1Hz:
			sys->LED1_timer = sys->h_timer2; // use h_timer2 as the LED1 timer
			if (sys->begin_LED1) {
				HAL_GPIO_WritePin(sys->h_GPIO_LED, sys->LEDPin1, GPIO_PIN_RESET); // toggle pin
				sys->begin_LED1 = FALSE;
			}
			else {
				sys->begin_LED1 = TRUE;
			}
			break;
		case LED2_5Hz:
			sys->LED2_timer = sys->h_timer1; // use h_timer1 as the LED2 timer
			sys->begin_LED2 = !sys->begin_LED2;
			if (sys->begin_LED2) {
				HAL_GPIO_WritePin(sys->h_GPIO_LED, sys->LEDPin2, GPIO_PIN_RESET); // toggle pin
				sys->begin_LED2 = FALSE;
			}
			else {
				sys->begin_LED2 = TRUE;
			}
			break;
		case LED2_1Hz:
			sys->LED2_timer = sys->h_timer2; // use h_timer2 as the LED2 timer
			if (sys->begin_LED2) {
				HAL_GPIO_WritePin(sys->h_GPIO_LED, sys->LEDPin2, GPIO_PIN_RESET); // toggle pin
				sys->begin_LED2 = FALSE;
			}
			else {
				sys->begin_LED2 = TRUE;
			}
			break;

		case HANDSHAKE_ARE_YOU_THERE:
			HAL_UART_Transmit_IT(&huart3, (uint8_t*)"Are you still there?\n", sizeof("Are you still there?\n"));
			break;
		}
		HAL_UART_Receive_IT(&huart3, sys->incomingByte, SIZE_INCOMING_DATA); // allow for more receives

	}

}
