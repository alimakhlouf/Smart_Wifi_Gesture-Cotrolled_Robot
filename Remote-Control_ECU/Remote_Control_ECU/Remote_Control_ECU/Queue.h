/*
 * linked_list.h
 *
 *  Created on: ?? —Ã»° ???? Â‹
 *      Author: hppp
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdlib.h>
#include <stdint.h>



typedef struct node{
	void * p_element;
	struct node * next;
}node_t;


typedef struct start{
	node_t * front;
	node_t * rear;
	volatile uint16_t size;
} start_t;


start_t * create_list (void);
uint8_t list_empty(start_t * st);
uint8_t get_size(start_t * st);
void push(volatile start_t *st, void * element, uint8_t element_size);
uint8_t pop(start_t * st, void * popped_e, uint16_t element_size);
void clear_list(start_t *st);







#endif /* LINKED_LIST_H_ */
