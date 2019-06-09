#include <base/listnode.h>
#include <base/nodelist.h>

BNodeList :: BNodeList()
{
	this->first  = NULL;
	this->length = 0;
}

BNodeList :: BNodeList(BListNode *initial)
{
	this->first  = initial;
	this->length = BListNode::length(initial);
}

BListNode *BNodeList :: get(uint32 index)
{
	return BListNode::get(this->first, (int32) index);
}

int32 BNodeList :: index_of(BListNode* item)
{
	int32 index = 0;

	for (BListNode *current = this->first; current != NULL; current = current->next, index++)
	{
		if (current == item) return index;
	}

	return -1;
}

void BNodeList :: add(BListNode *node)
{
	BListNode::append(node, &this->first);
	this->length++;
}

void BNodeList :: insert(BListNode* item, uint32 index)
{
	BListNode *preceding = BListNode::get(this->first, index);
	BListNode::insert(item, preceding);

	this->length++;
}

BListNode* BNodeList::remove(uint32 index)
{
}
