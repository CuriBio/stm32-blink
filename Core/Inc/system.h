/*
 * system.h
 *
 *  Created on: Apr 19, 2023
 *      Author: achar
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "stm32h7xx_hal.h"
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "communicator.h"

#define SIZE_INCOMING_DATA 1
typedef struct
{
	TIM_HandleTypeDef *h_timer1;
	TIM_HandleTypeDef *h_timer2;
	GPIO_TypeDef *h_GPIO_LED;

	uint16_t LEDPin1;
	uint16_t LEDPin2;

	uint8_t LEDToggle1;
	uint8_t LEDToggle2;
	Command_t LED1_State;
	Command_t LED2_State;

	TIM_HandleTypeDef *LED1_timer;
	TIM_HandleTypeDef *LED2_timer;
	TIM_HandleTypeDef *handshakeTimer;
	uint8_t *incomingByte;

	bool_t tx_complete;

	RingBuffer_t *receive_ring_buffer;
} System;


void init_system(System *sys,
		TIM_HandleTypeDef *h_timer1,
		TIM_HandleTypeDef *h_timer2,
		TIM_HandleTypeDef *handshakeTimer,
		GPIO_TypeDef *h_GPIO,
		uint16_t LEDPin1, uint16_t LEDPin2,
		uint16_t size_of_receive_buffer);
void system_main(System *sys);
void commandHandler(System *sys);
#endif /* INC_SYSTEM_H_ */
