#ifndef __BNODELIST_H__
#define __BNODELIST_H__

#include <osndef.h>
#include "listnode.h"

class BNodeList
{
public:
	BListNode *first;
	uint32     length;

public:
	BNodeList();
	BNodeList(BListNode *initial);

	BListNode *get(uint32 index);

	int32 index_of(BListNode *item);		// Returns the index of the first occurence of the given item in the list. Returns -1 if not found.

	/* Adding and removing */
	void add(BListNode *item);
	void insert(BListNode *item, int32 index);
	BListNode *remove(int32 index);
	BListNode *snip(int32 index);
};

#endif
