/*
 * linked_list.c
 *
 *  Created on: ?? —Ã»° ???? Â‹
 *      Author: hppp
 */

//functions for linked list:
#include "Queue.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define ERROR -1

static void * create_node (void * entry, uint8_t size);


//1- create list: size and a pointer to null
// the dynamic allocation happens inside this file not in the application file

start_t * create_list()
{
	start_t * st =  (start_t *)malloc(sizeof(start_t));
	st->front = NULL;
	st->rear = NULL;
	st->size = 0;
	return st;
}

static void * create_node(void * entry, uint8_t size)
{
	node_t * tmp_node = malloc(sizeof(node_t));//allocate a node to be added to the list
	//allocate an element that will contain the element data
	tmp_node->p_element = malloc(size);
	//copy the data in *entry to the element side
	memcpy(tmp_node->p_element, entry, size);//(destination, source, element_size)

	tmp_node->next = NULL;

	return tmp_node;
}

uint8_t list_empty(start_t * st)
{
    if (st->size == 0)
        return 1;
    else
        return 0;

}

uint8_t get_size(start_t * st)
{
    return st->size;
}

void push(volatile start_t * st, void * element, uint8_t element_size)
{
    node_t * p_new_node = create_node(element, element_size);

    if (st->size == 0) // if its empty .. put one node and make the front and the rear point at it
    {
        st->front = p_new_node;
        st->rear = p_new_node;
    }
    else //if it's not empty ... make the add it at the rear side and make the rear point at it
    {
        st->rear->next = p_new_node;
        st->rear = st->rear->next;
    }

    st->size += 1;
}

uint8_t pop(start_t * const st, void * popped_e, uint16_t element_size)
{

    if (st->size == 0)
    {
        return ERROR;
    }
    else
    {
        
		
		//copy the front node's element to the popped_e .. because we are going to free the front node after pop operation finishes
		memcpy(popped_e, st->front->p_element, element_size);
		
		//define a pointer that points to the front node
		node_t * tmp_node = st->front;
        //make the front pointer points to the next node
        st->front = st->front->next;
		
		//free the node that was popped
		free(tmp_node->p_element);
		free(tmp_node);
		
        (st->size) -= 1;

        return 1;
    }

}


void clear_list(start_t *st)
{

	//else
	node_t * p_tmp = st->front;
	node_t * p_tmp2 = st->front;// used to point to the node p_next to the deleted node
								  //because after the current node is deleted, it won't be
								  //able to go to the p_next node

	do
	{
		p_tmp2 = p_tmp2->next; // move the pointer to the next node because i
		free(p_tmp->p_element);
		free(p_tmp);
		p_tmp = p_tmp2;
	}
	while (p_tmp); // while it didn't reach the NULL part

    st->size = 0;
}


