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
#include <stdbool.h>


/*a linked list node structure contains a pointer to the element of the node
  and a pointer to the next node
*/
/* --------------------------------------------------------------------------
Structure Name]: linked_node
[Structure Description]:
This structure is responsible for maintaining information about linked list
node structure that contains a pointer to the element of the node and
a pointer to the next node 
-----------------------------------------------------------------------------*/
typedef struct linked_node{
	void * p_element;
	struct linked_node * next;
}linked_node_t;

/* --------------------------------------------------------------------------
Structure Name]: node
[Structure Description]:
This structure is responsible for maintaining information about the Queue list
structure of type linked list that a rear, front pointer and a list size 
variable
-----------------------------------------------------------------------------*/
typedef struct q_list{
	linked_node_t * front;// points to the oldest node in the list 
	linked_node_t * rear; // points to the last pushed node
	uint16_t size;  // indicates the size of the list
}q_list_t;

/* -----------------------------------------------------------------------------
[Function Name] : create_list
[Description] : This function is responsible constructing a Queue list:
1- it allocates a structure node that contains front, rear pointers , and size 
variable
[Returns] : this returns the allocated structure node.
------------------------------------------------------------------------------*/
q_list_t * q_create_list (); //,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RETURN,,,,,,,,,,,,,,


/* -----------------------------------------------------------------------------
[Function Name] : q_is_empty
[Description] : This function checks whether a q list is empty or not
[Args] :
[in] -q_list_t * ap_list
	this argument is responsible for pointing at the list structure node that will 
	be tested
[Returns] : this returns zero(false) if non empty, and a non-zero(true) value 
            if empty
------------------------------------------------------------------------------*/
bool q_is_empty(q_list_t * ap_list);


/* -----------------------------------------------------------------------------
[Function Name] : get_size
[Description] : This function checks returns the size of the list
[Args] :
[in] -q_list_t * ap_list
	this argument is responsible for pointing at the list structure node
[Returns] : this returns the size of the list
------------------------------------------------------------------------------*/
uint8_t get_size(q_list_t * ap_list);

/* -----------------------------------------------------------------------------
[Function Name] : q_push
[Description] : pushes a new element to the Queue list
[Args] :
[out] -q_list_t * ap_list
	this argument is responsible for pointing at the list structure node
[in] -void * a_element
	this argument is responsible for pointing at the element that'll be pushed
[in] -uint8_t a_element_size
	this argument is responsible for indicates the element size
------------------------------------------------------------------------------*/
void q_push(q_list_t * ap_list, void * ap_element, uint8_t a_element_size);//,,,,,,,,,,,,,,,,,,RETURN,,,,,,,,,,,,,,,,,,,,,,,,

/* -----------------------------------------------------------------------------
[Function Name] : q_pop
[Description] : pops an element from the front of the Queue list 
[Args] :
[out] -q_list_t * ap_list
	this argument is responsible for pointing at the list structure node
[out] -void * ap_popped_e
	this argument is responsible for pointing at the element that'll be popped
[in] -uint8_t a_element_size
	this argument is responsible for indicates the element size that'll be popped
[Returns] : the return indicates the state of the operation
ERROR : failure
TRUE  : success
------------------------------------------------------------------------------*/
uint8_t q_pop(q_list_t * ap_list, void * ap_poppped_e, uint16_t element_size);

/* -----------------------------------------------------------------------------
[Function Name] : q_clear_list
[Description] : clears the Queue list
[Args] :
[out] -q_list_t * ap_list
	this argument is responsible for pointing at the list structure node that'll be
	cleared
------------------------------------------------------------------------------*/
void q_clear_list(q_list_t * ap_list);

#endif /* LINKED_LIST_H_ */
