/*
 * it.c
 *
 *  Created on: Apr 19, 2023
 *      Author: achar
 */

#include "system.h"
//#include "communicator.h"

extern System my_sys;
extern UART_HandleTypeDef huart3;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == (&my_sys)->LED1_timer) {
		(&my_sys)->LEDToggle1 = 1; // flip flag
	}
	if (htim == (&my_sys)->LED2_timer) {
		(&my_sys)->LEDToggle2 = 1; // flip flag
	}
	if (htim == (&my_sys)->handshakeTimer) {
		my_sys.receive_ring_buffer->push(my_sys.receive_ring_buffer, HANDSHAKE_ARE_YOU_THERE); // add a command to the ring buffer

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if (huart==&huart3) {
		my_sys.handshakeTimer->Instance->CNT=0;
//		my_sys.received_input = TRUE;
		my_sys.receive_ring_buffer->push(my_sys.receive_ring_buffer, *my_sys.incomingByte);


	}

}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

	if (huart==&huart3) {
//		my_sys.tx_complete = TRUE;
//		my_sys.received_input = FALSE;
	}

}

