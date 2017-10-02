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
	/* This function returns a pointer to the ->next pointer	*
	 * of the node previous to the node of the given index.		*
	 * This can be useful because it enables you to 			*
	 * insert/add/remove a node from the list using just one 	*
	 * pointer.													*/
	
	if (index == -1 && this->m_length == 0) return &this->m_first;
	
	if (! (index >= -(this->m_length) && index <= (this->m_length - 1))) return NULL;	// If the index is out of range
	
	if (index >= 0)
	{
		BSListNode **current = &this->m_first;
		
		for (int32 i = 0; i <= index && *current != NULL; i++)
		{
			current = &(*current)->next;
		}
		
		return current;
	}
	else
	{
		/* Negative list indexes count from the end of the list.	*
		 * Index -1 would be the last element in the list,			*
		 * -2 the penultimate, and so on. Adding the list's length	*
		 * to the negative index (as is done here) gets us the cor-	*
		 * -responding positive index.								*/
		
		return this->ptr_to_index(index + this->m_length);
	}
}

void BSList :: add_node(BSListNode *node)
{
	/* Adds the given node to the end of the list.		*
	 * The node is freed when the destructor is called,	*
	 * but not its ->data.								*/
	
	BSListNode **last = this->ptr_to_index(-1);
	
	*last      = node;
	node->next = NULL;
	
	this->m_length++;
}

void BSList :: add(void *item)
{
	/* Create a node for the item */
	
	BSListNode *node = (BSListNode *) malloc(sizeof(BSListNode));
	node->data = item;
	node->next = NULL;
	
	/* Add it to the list */
	this->add_node(node);
}
