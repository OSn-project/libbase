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
	this->length = BListNode::length(this->first);
}

BListNode *BNodeList :: get(uint32 index)
{
	return BListNode::get(this->first, (int32) index);
}

void BNodeList :: add(BListNode *node)
{
	BListNode::append(node, &this->first);
	this->length++;
}
