#ifndef __BLIST_H__
#define __BLIST_H__

#include <osndef.h>
#include "nodelist.h.h"

template <typename T>
class BList : public BNodeList
{
public:
	/* Type-safe methods that override their BNodeList counterparts */
	
	inline T *get(uint32 index) { return (T *) this->BNodeList::get(index); }
};

#endif
