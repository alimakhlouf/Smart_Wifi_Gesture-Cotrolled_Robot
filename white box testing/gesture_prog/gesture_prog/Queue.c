/*
 * linked_liap_list.c
 *
 *  Created on: ?? —Ã»° ???? Â‹
 *      Author: hppp
 */

//functions for linked liap_list:
#include "Queue.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define ERROR -1
#define TRUE 1

/* -----------------------------------------------------------------------------
[Function Name] : create_node
[Description] : This function is responsible for allocating a new Queue node

[Args] :
[in] -void * ap_entry
this argument points at the entry that'll be included in the
created node
[in] -uint8_t a_size
this argument indicates the size of the entry that'll be added
[Returns] : pointer to the node that was created
------------------------------------------------------------------------------*/
static void * create_node (void * ap_entry, uint8_t a_size);


//1- create liap_list: a_size and a pointer to null
// the dynamic allocation happens inside this file not in the application file
q_list_t * q_create_list()
{
	//allocate a place for the start node
	q_list_t * ap_list =  malloc(sizeof(q_list_t));
	
	//initializing the elements of it
	ap_list->front = NULL;
	ap_list->rear = NULL;
	ap_list->size = 0;
	
	return ap_list;
}

static void * create_node(void * entry, uint8_t a_size)
{
	//allocate a node to be added to the list
	linked_node_t * tmp_node = malloc(sizeof(linked_node_t));//allocate a node to be added to the list
	
	//allocate an element that will contain the entry data
	tmp_node->p_element = malloc(a_size);
	//copy the data in *entry to the element side
	memcpy(tmp_node->p_element, entry, a_size);//(destination, source, a_el_size)

	tmp_node->next = NULL;

	return tmp_node;
}

bool q_is_empty(q_list_t * ap_list)
{
    return (ap_list->size == 0);
}

uint8_t get_a_size(q_list_t * ap_list)
{
    return ap_list->size;
}

void q_push(q_list_t *ap_list, void * element, uint8_t a_el_size)
{
	//create a node that'll be contain the element and pushed to the list
    linked_node_t * p_new_node = create_node(element, a_el_size);

	//if its empty ..make the front and the rear point at the added node
    if (ap_list->size == 0) 
    {
        ap_list->front = p_new_node;
        ap_list->rear = p_new_node;
    }//if it's not empty ... add it at the rear side(make the rear point at it)
    else 
    {
        ap_list->rear->next = p_new_node;
        ap_list->rear = ap_list->rear->next;
    }
	
	//increment the size of the list by one
    ap_list->size += 1;
}

uint8_t q_pop(q_list_t * ap_list, void * popped_e, uint16_t a_el_size)
{
    //if it's empty .. return error
    if (ap_list->size == 0)
    {
        return ERROR;
    }
    else
    {
		/*copy the front node's element to the popped_e because we are going to 
		 free the front node after pop operation finishes*/
		memcpy(popped_e, ap_list->front->p_element, a_el_size);
		
		//define a pointer that points to the front node
		linked_node_t * tmp_node = ap_list->front;
		
        //make the front pointer points to the next node
        ap_list->front = ap_list->front->next;
		
		//free the node that was popped
		free(tmp_node->p_element);
		free(tmp_node);
		
		//decrement the list size
        ap_list->size -= 1;

        return TRUE;
    }

}


void q_clear_list(q_list_t *ap_list)
{
	linked_node_t * p_tmp = ap_list->front;
	/* 
	   used to point to the node next to the deleted node because after the
	   current node is deleted, it won't be able to go to the next node
	*/
	linked_node_t * p_tmp2 = ap_list->front;
					
	do
	{
		/*move the p_tmp2 to the next node .. for when p_tmp frees the node
		it can find the next node by pointing to the same node pointed by 
		p_tmp2*/ 
		p_tmp2 = p_tmp2->next; 
		
		//free the element allocated inside the allocated node
		free(p_tmp->p_element);
		//free the element allocated node
		free(p_tmp);
		//move to the next node
		p_tmp = p_tmp2;
	}
	while (p_tmp); // while it didn't reach the NULL part
	
	//clear the size variable
    ap_list->size = 0;
}


