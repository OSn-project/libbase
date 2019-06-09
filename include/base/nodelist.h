#ifndef BASE_NODELIST_H_
#define BASE_NODELIST_H_

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
	int32 index_of(BListNode *item);		// Returns the index of the given item in the list. Returns -1 if not found.

	/* Adding and removing */
	void add(BListNode *item);
	void insert(BListNode *item, uint32 index);
	BListNode *remove(uint32 index);
};

#include "list.h"

#endif
