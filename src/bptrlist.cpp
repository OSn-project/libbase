/* A dually linked list of pointers. This linked list	*
 * stores its length in a variable, so that the list	*
 * doesn't need to be iterated over to get the length	*
 * every time a condition needs to be checked.			*/

#include <stdlib.h>

#include "../include/osndef.h"
#include "../include/bptrlist.h"

BVoidPtrList :: BVoidPtrList()
{
	this->m_length = 0;
	
	this->first = NULL;
	this->last  = NULL;
}

BVoidPtrList :: ~BVoidPtrList()
{
	this->clear();
}

size_t BVoidPtrList :: length()
{
	return this->m_length;
}

void BVoidPtrList :: clear()
{
	BPtrListNode *current = this->first, *next;
	
	while (current != NULL)
	{
		next = (BPtrListNode *) current->next;
		free(current);
		current = next;
	}
	
	this->m_length = 0;
	
	this->first = NULL;
}

void BVoidPtrList :: append(void *item)
{
	/* Wrapper around ->append_node that creates the node for you */
	
	/* Construct the node */
	BPtrListNode *node = (BPtrListNode *) calloc(1, sizeof(BPtrListNode));
	node->item = item;
	
	/* Append it */
	this->append_node(node);
}

void BVoidPtrList :: append_node(BPtrListNode *node)
{
	/* Add the node */
	if (this->first == NULL)
	{
		/* If the list is empty then this will	*
		 * be both the first and last node.		*/
		
		this->first = node;
		this->last  = node;
	}
	else
	{
		BListNode::append(node, this->first);
		this->last  = node;
	}
	
	/* Increment the list length */
	this->m_length++;
}
