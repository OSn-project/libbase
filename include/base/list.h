#ifndef BLIST_H_
#define BLIST_H_

#include <osndef.h>
//#include "nodelist.h"

template <typename NodeType>
class BList : private BNodeList
{
public:
	/* Type-safe methods that override their BNodeList counterparts */
	
	inline BList() : BNodeList() {}
	inline BList(NodeType *initial) : BNodeList(initial) {}

	inline NodeType *get(uint32 index) 					{ return (NodeType *) this->BNodeList::get(index); }
	inline int32 index_of(NodeType *node)				{ return this->BNodeList::index_of(node); }

	/* Adding and removing */
	inline void add(NodeType *item)						{ return this->BNodeList::add(item); }
	inline void insert(NodeType *item, uint32 index)	{ return this->BNodeList::insert(item, index); }
	inline NodeType *remove(uint32 index)				{ return (NodeType *) this->BNodeList::remove(index); }
};

#endif
