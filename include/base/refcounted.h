#ifndef __BREFCOUNTED_H__
#define __BREFCOUNTED_H__

#include <stdio.h>
#include <osndef.h>

class BRefCounted
{
	uint32 ref_count;

public:
	BRefCounted();

	friend void ref(BRefCounted *obj);
	friend void unref(BRefCounted *obj);
	
	uint32 get_refcount();
};

void ref(BRefCounted *obj);
void unref(BRefCounted *obj);

#endif
