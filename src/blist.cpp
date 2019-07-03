#include "../include/base/list.h"

BBaseList :: BBaseList()
{
	this->m_first  = NULL;
	this->m_length = 0;
}

BBaseList :: BBaseList(BListNode *initial)
{
	this->m_first  = initial;
	this->m_length = BListNode::length(initial);
}

BListNode *BBaseList :: get(uint32 index)
{
	return BListNode::get(this->m_first, (int32) index);
}

int32 BBaseList :: index_of(BListNode* item)
{
	int32 index = 0;

	for (BListNode *current = this->m_first; current != NULL; current = current->next, index++)
	{
		if (current == item) return index;
	}

	return -1;
}

void BBaseList :: add(BListNode *node)
{
	BListNode::append(node, &this->m_first);
	this->m_length++;
}

void BBaseList :: insert(BListNode* item, uint32 index)
{
	if (index == 0)
	{
		this->m_first = BListNode::insert(item, this->m_first);
	}
	else
	{
		BListNode *preceding = BListNode::get(this->m_first, index);
		BListNode::insert(item, preceding);
	}

	this->m_length++;
}

BListNode* BBaseList::remove(uint32 index)
{
}
