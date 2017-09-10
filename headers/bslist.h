#ifndef __BSLIST_H__
#define __BSLIST_H__

#include "bdefs.h"

struct BSListNode
{
	void       *data;
	BSListNode *next;
};

class BSList
{	
	BSListNode  *__getlast();
	
public:
	
	BSListNode  *first;
	
	BSList();
	~BSList();

	void *get(uint32 index);
	uint32 length();
	
	/* Adding and removing */
	void add(void *item);
	void insert(void *item, int32 index);
	void remove(void *item);
	void remove_at(int32 index);
	void reset();
	
	/* Slicing */
	BSList *slice(int32 start, int32 end);
};

#endif
