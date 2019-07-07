#include <base/listnode.h>

BListNode :: BListNode()
{
	this->next = NULL;
	this->prev = NULL;
}

BListNode *BListNode :: last (BListNode *node)
{
	while (node->next != NULL)
	{
		node = node->next;
	}

	return node;
}

BListNode *BListNode :: first(BListNode *node)
{
	while (node->prev != NULL)
	{
		node = node->prev;
	}

	return node;
}

uint32 BListNode :: preceding(BListNode *node)
{
	uint32 count;

	for (count = 0; node->prev != NULL; node = node->prev)
	{
		count++;
	}

	return count;
}

uint32 BListNode :: following(BListNode *node)
{
	uint32 count;

	for (count = 0; node->next != NULL; node = node->next)
	{
		count++;
	}

	return count;
}

void BListNode :: foreach (BListNode *first, void (*func)(BListNode *node))
{
	for (BListNode *current = first; current != NULL; current = current->next)
	{
		func(current);
	}
}

void BListNode :: foreach (BListNode *first, void (*func)(BListNode *node, void *data), void *data)
{
	for (BListNode *current = first; current != NULL; current = current->next)
	{
		func(current, data);
	}
}

BListNode *BListNode :: insert(BListNode *add, BListNode *current)
{
	/* Insert `add` before `node` */
	add->next = current;
	add->prev = current->prev;		// Works when `current->prev` is NULL

	if (add->prev)
		add->prev->next = add;
	current->prev = add;
}

void BListNode :: append(BListNode *new_last, BListNode **list)
{
	if (*list == NULL)
	{
		/* If the list is empty */
		*list = new_last;
	}
	else
	{
		BListNode::append(new_last, *list);
	}
}

void BListNode :: append(BListNode *new_last, BListNode *node)
{
	BListNode *old_last = BListNode::last(node);

	old_last->next = new_last;
	new_last->prev = old_last;

	new_last->next = NULL;
}

void BListNode :: rip(BListNode *node)
{
	/* Switch the pointers of the surrounding nodes (if present) to point to each other */
	if (node->next)
		node->next->prev = node->prev;
	if (node->prev)
		node->prev->next = node->next;
}

void BListNode :: remove(BListNode *node)
{
	BListNode::rip(node);
	
	/* NULL the node's pointers */
	node->next = NULL;
	node->prev = NULL;
}

BListNode *BListNode :: get(BListNode *node, int32 index)
{
	if (index == 0) return node;

	BListNode *current = node;

	if (index > 0)
	{
		for (int32 i = 0; i < index; i++)
		{
			if (current == NULL) return NULL;	// TODO: Cleaner solution for out-of-bounds?
			
			current = current->next;
		}
	}
	else
	{
		for (int32 i = 0; i > index; i--)
		{
			if (current == NULL) return NULL;
			
			current = current->prev;
		}
	}

	return current;
}

void BListNode :: replace(BListNode *current, BListNode *target)
{
	/* Rip the current node out of its list */
	BListNode::rip(current);
	
	/* Change the surrounding pointers	*
	 * to point to the new node.		*/
	if (current->next)
		current->next->prev = target;
	if (current->prev)
		current->prev->next = target;
	
	/* Change the new node's pointers to point	*
	 * to its surrounding nodes.				*/
	target->next = current->next;
	target->prev = current->prev;
	
	/* Clear the old node's pointers */
	current->next = NULL;
	current->prev = NULL;
}
