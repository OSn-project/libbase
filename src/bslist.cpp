#include <stdlib.h>

#include "../headers/bdefs.h"
#include "../headers/bslist.h"

BSList :: BSList ()
{
	this->first = NULL;
}

BSList :: ~BSList ()
{
	this->reset();
}

void BSList :: reset ()
{
	/* Frees all the nodes in the list */
	
	for (BSListNode *current = this->first, *next; current != NULL;)
	{
		next = current->next;
		free(current);
		current = next;
	}
	
	this->first = NULL;		// Set the first pointer to NULL to signify that the list is empyt
}

void BSList :: add(void *item)
{
	/* Create a node for the item */
	
	BSListNode *node = (BSListNode *) malloc(sizeof(BSListNode));
	node->data = item;
	
	/* Add it to the list */	
	
	/* The technique used here is described very well 	*
	 * by Prof. Brailsford in this video:				*
	 * 													*
	 *           https://youtu.be/1s0w_p5HEuY			*/
		
	BSListNode **current = &this->first;
	
	while (*current != NULL)
	{
		current = &(*current)->next;
	}
	
	node->next = *current;
	*current   =  node;
}
