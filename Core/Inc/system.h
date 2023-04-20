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

typedef struct
{
	TIM_HandleTypeDef *h_timer1;
	TIM_HandleTypeDef *h_timer2;
	GPIO_TypeDef *h_GPIO_LED;

	uint16_t LEDPin1;
	uint16_t LEDPin2;

	uint8_t LEDToggle1;
	uint8_t LEDToggle2;
} System;


void init_system(System *sys, TIM_HandleTypeDef *h_timer1, TIM_HandleTypeDef *h_timer2, GPIO_TypeDef *h_GPIO, uint16_t LEDPin1, uint16_t LEDPin2);
void system_main(System *sys);
#endif /* INC_SYSTEM_H_ */
