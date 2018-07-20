#include <base/refcounted.h>

BRefCounted :: BRefCounted()
{
	this->ref_count = 1;
}

uint32 BRefCounted :: get_refcount()
{
	return this->ref_count;
}

void ref(BRefCounted *obj)
{
	/* Increase an object's reference count */
	
	obj->ref_count++;
}

void unref(BRefCounted *obj)
{
	/* Increase an object's reference count */
	
	obj->ref_count--;
	
	/* Delete the object if its last reference was lost */
	if (obj->ref_count == 0)
	{
		delete obj;
	}
}
