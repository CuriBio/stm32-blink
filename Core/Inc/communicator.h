/*
 * communicator.h
 *
 *  Created on: Apr 21, 2023
 *      Author: achar
 */

#ifndef INC_COMMUNICATOR_H_
#define INC_COMMUNICATOR_H_

#include "main.h"
//#include "system.h"
extern UART_HandleTypeDef huart3;



/***************	RING BUFFER DEFINITIONS AND DECLARATIONS	***************/

typedef struct __RingBuffer {
	uint16_t head; // write position
	uint16_t tail; // read position
	uint16_t max; // size of buffer
	uint8_t full; // flag if buffer is full
	uint8_t* data; // buffer

	uint8_t (*push)(struct __RingBuffer * buffer_s, uint8_t newData); // write data to buffer
	uint8_t (*pop)(struct __RingBuffer * buffer_s, uint8_t * dataPointer); // read data from buffer
	uint16_t (*get_total_elements)(struct __RingBuffer * buffer_s); // get number of readable elements in buffer
	uint8_t (*is_empty)(struct __RingBuffer * buffer_s); // if buffer is empty (equivalent to if get_total_elements is 0)
} RingBuffer_t;

typedef enum Command_t{
	LED1_5Hz 	= 	97, // 'a'
	LED1_1Hz 	= 	115, // 's'
	LED2_5Hz 	= 	100, // 'd'
	LED2_1Hz 	= 	102 // 'f'
} Command_t;

RingBuffer_t * ring_buffer_init(uint32_t size_buffer /*, Thread_Code_t this_thread_code*/);
uint8_t push_ring_buffer(RingBuffer_t * buffer_s, uint8_t newData);
uint8_t pop_ring_buffer(RingBuffer_t * buffer_s, uint8_t * dataPointer);
uint16_t total_elements_ring_buffer(RingBuffer_t * buffer_s);
uint8_t is_empty_ring_buffer(RingBuffer_t * buffer_s);

#endif /* INC_COMMUNICATOR_H_ */
