/* The technique used in the implementation of this linked			*
 * list is described very well by Prof. Brailsford in this video:	*
 *           		https://youtu.be/1s0w_p5HEuY					*
 * This linked list stores its length in a variable, to improve 	*
 * performance with longer lists. 									*/

#include <stdlib.h>

#include "../headers/bdefs.h"
#include "../headers/bslist.h"

BSList :: BSList ()
{
	this->m_first  = NULL;
	this->m_length = 0;
}

BSList :: ~BSList ()
{
	this->reset();
}

void BSList :: reset ()
{
	/* Frees all the nodes in the list */
	
	for (BSListNode *current = this->m_first, *next; current != NULL;)
	{
		next = current->next;
		free(current);
		current = next;
	}
	
	this->m_first  = NULL;		// Set the first pointer to NULL to signify that the list is empty
	this->m_length = 0;
}

BSListNode **BSList :: ptr_to_index(int32 index)
{
	if (-(this->m_length) < index < this->m_length - 1)
	
	if (index >= 0)
	{
		BSListNode **current = &this->m_first;
		
		for (int32 i = 0; i < index && *current != NULL; i++)
		{
			current = &(*current)->next;
		}
		
		return current;
	}
	else
	{
		/* Negative list indexes count from the end of the list.	*
		 * Index -1 would be the last element in the list,			*
		 * -2 the penultimate, and so on.							*/
		
	}
}

void BSList :: add(void *item)
{
	/* Create a node for the item */
	
	BSListNode *node = (BSListNode *) malloc(sizeof(BSListNode));
	node->data = item;
	
	/* Add it to the list */	
			
	BSListNode **last = this->ptr_to_index(-1);
	
	node->next = *current;
	*current   =  node;
	
	this->m_length++;
}
