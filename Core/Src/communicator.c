/*
 * communicator.c
 *
 *  Created on: Apr 21, 2023
 *      Author: achar
 */

#include "communicator.h"
//#include "stdlib.h"

RingBuffer_t * ring_buffer_init(uint32_t size_buffer /*, Thread_Code_t this_thread_code*/) {
	RingBuffer_t * buffer_s = (RingBuffer_t*)malloc(sizeof(RingBuffer_t));
//	if (this_RawRingBuffer == NULL)
//	{
//		main_code_check(MAIN_STATUS_MEMORY_FAILURE, this_thread_code);
//	}

	buffer_s->data = (uint8_t*)malloc(sizeof(uint8_t*)*size_buffer);
//	if (this_RawRingBuffer->data == NULL)
//	{
//		main_code_check (MAIN_STATUS_MEMORY_FAILURE, this_thread_code);
//	}
	buffer_s->head = 0; // write into position
	buffer_s->tail = 0; // read from position
	buffer_s->full = 0; // whether full or not
	buffer_s->max = size_buffer; // size of buffer
	buffer_s->pop = pop_ring_buffer; // pop method
	buffer_s->push = push_ring_buffer; // push method
	buffer_s->get_total_elements = total_elements_ring_buffer;
	buffer_s->is_empty = is_empty_ring_buffer;

	return buffer_s;
}

/*			push_ring_buffer
 * push data to buffer, store at head
 * status code return 1 if buffer is full, no push
 * status code return 0 for store
 *
 */
uint8_t push_ring_buffer(RingBuffer_t * buffer_s, Command_t newData) {

	if (buffer_s->full) { // if buffer is full, do not add to buffer
		return (uint8_t) 1; // return status flag
	}
	buffer_s->data[buffer_s->head] = newData; // add data at head
	buffer_s->head = (buffer_s->head == buffer_s->max-1 ? 0 : buffer_s->head+1); // increment or reset head
	buffer_s->full = (buffer_s->head==buffer_s->tail); // check if buffer is full
	return (uint8_t) 0; // return status flag

/*	mantarray main code -->
	uint8_t full_return = 1;
	if(!buffer_s->full)
	{
		buffer_s->data[buffer_s->head] = newData;
		buffer_s->head = (buffer_s->head == buffer_s->max-1 ?
									0 : buffer_s->head + 1);
		buffer_s->full = (buffer_s->head == buffer_s->tail);
		full_return=0;
	}
	return full_return;

	//TODO Data overwrite?
	/*
	if(thisRingBuffer->full)
	{
	thisRingBuffer->tail = (thisRingBuffer->tail + 1) % thisRingBuffer->max;
	}
	*/

}

/*			pop_ring_buffer
 * read data from buffer (first in), read from tail
 * status code return 1 for empty buffer, no read
 * status code return 0 for read
 */
uint8_t pop_ring_buffer(RingBuffer_t * buffer_s, uint8_t * dataPointer) {
	// if head is at tail, and not full, treat it as an empty buffer and do nothing
	if (is_empty_ring_buffer(buffer_s)) {
		return (uint8_t) 1; // retrun status flag
	}
	*dataPointer = buffer_s->data[buffer_s->tail]; // retrieve data at tail, store at pointer location (data variable)
	buffer_s->full = 0; // automatically not full
	buffer_s->tail = (buffer_s->tail+1) % buffer_s->max; // increment, or reset if at max (i.e. remainder of max with max is 0)
	return (uint8_t) 0;

/*  mantarray main code -->
	uint8_t empty_return = 1;
	if(buffer_s->full || (buffer_s->head != buffer_s->tail))
	{
		*data = buffer_s->data[buffer_s->tail];
		buffer_s->full = 0;
		buffer_s->tail = (buffer_s->tail + 1) % buffer_s->max;
		empty_return = 0;
	}
	return empty_return;
*/
}


/* 			total_elements_ring_buffer
 * find number of stored, unused elements in ring buffer
 * return is number of elements (head - tail)
 * maximum is size of buffer (buffer is full)
 * minimum is 0 (buffer is empty)
 */
uint16_t total_elements_ring_buffer(RingBuffer_t * buffer_s) {
	if (buffer_s->full) {
		return (uint16_t) buffer_s->max;
	}

	int16_t size = buffer_s->head - buffer_s->tail;
	if (size >= 0) {
		return size;
	}
	else {
		return (uint16_t) buffer_s->max + size;
	}


/* mantarray main code -->

	if (buffer_s->head >= buffer_s->tail) {
		return (buffer_s->head - buffer_s->tail);
	}
	int16_t size = thisRingBuffer->max;

	if(!thisRingBuffer->full) {
		if(thisRingBuffer->head >= thisRingBuffer->tail) {
			size = (thisRingBuffer->head - thisRingBuffer->tail);
		}
		else {
			size = (thisRingBuffer->max + thisRingBuffer->head - thisRingBuffer->tail);
		}
	}
	return size;
*/
}

/*			is_empty_ring_buffer
 * checks if ring buffer is empty
 * returns 1 if buffer is both not full and head and tail at same position (i.e.
 * returns 0 otherwise (i.e. full)
 */
uint8_t is_empty_ring_buffer(RingBuffer_t * buffer_s) {
    return (uint8_t)(!buffer_s->full && (buffer_s->head == buffer_s->tail));
}


