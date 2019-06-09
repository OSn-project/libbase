#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "privablic.h"

#include "../include/base/refcounted.h"

struct PROXY_ref_count { typedef uint32 (BRefCounted::*type); };
template class private_member<PROXY_ref_count, &BRefCounted::ref_count>;

SUITE(BRefCounted)
{
	TEST (Constructor)
	{
		BRefCounted *obj = new BRefCounted();
		
		CHECK(obj->*member<PROXY_ref_count>::value == 1);
		
		delete obj;
	}

	TEST (get_refcount)
	{
		BRefCounted *obj = new BRefCounted();
		
		obj->*member<PROXY_ref_count>::value = 0x72656663;
		uint32 ref_count = obj->get_refcount();
		
		CHECK(ref_count == 0x72656663);
				
		delete obj;
	}

	TEST (ref)
	{
		BRefCounted *obj = new BRefCounted();
		
		obj->*member<PROXY_ref_count>::value = 62;
		
		ref(obj);
		
		CHECK(obj->*member<PROXY_ref_count>::value == 63);
		
		delete obj;
	}

	TEST (unref)
	{
		BRefCounted *obj = new BRefCounted();
		
		obj->*member<PROXY_ref_count>::value = 48;
		
		unref(obj);
		
		CHECK(obj->*member<PROXY_ref_count>::value == 47);
		
		delete obj;
	}

/*	TEST (unref_Lethal)	// ! Don't know how to mock delete.
	{
		BRefCounted *obj = new BRefCounted();
		
		unref(obj);	// This should destroy the object because we've lost the only reference to it.
		
		CHECK(obj->*member<PROXY_ref_count>::value == 47);
		
		delete obj;
	}*/
}
