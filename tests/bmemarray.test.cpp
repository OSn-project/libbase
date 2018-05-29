#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "privablic.h"

#include "../include/barray.h"

/* Privablic */

struct item_size { typedef size_t (BMemArray::*type); };
template class private_member<item_size, &BMemArray::item_size>;

struct size { typedef uint32 (BMemArray::*type); };
template class private_member<size, &BMemArray::size>;

struct capacity { typedef uint32 (BMemArray::*type); };
template class private_member<capacity, &BMemArray::capacity>;

struct data { typedef uint8 *(BMemArray::*type); };
template class private_member<data, &BMemArray::data>;

struct grow { typedef void (BMemArray::*type)(); };
template class private_member<grow, &BMemArray::grow>;

SUITE(BMemArray)
{
	TEST (Constructor)
	{
		MockRepository mocks;
		
		mocks.ExpectCallFunc(calloc).Return((void *) 0x1234);
		
		BMemArray *arr = new BMemArray(3, 17);
		
		CHECK(arr->*member<item_size>::value == 3);		
		CHECK(arr->*member<size>::value == 0);
		CHECK(arr->*member<capacity>::value == 17);
		
		CHECK(arr->*member<data>::value == (uint8 *) 0x1234);
		
		mocks.ExpectCallFunc(free).With((void *) 0x1234);
		delete arr;
	}
	
	TEST (add)
	{
		BMemArray arr(sizeof(uint16));
		
		uint16 test_data[] = {15, 12000};
		
		arr.add(&test_data[0]);	// Give it the address of the value we want to copy
		CHECK(arr.*member<size>::value == 1);

		arr.add(&test_data[1]);
		CHECK(arr.*member<size>::value == 2);
		
		CHECK(memcmp(test_data, arr.*member<data>::value, sizeof(test_data)) == 0);
		
		BArray<short> int_array;
		short i = 'hi';
		int_array.add(i);
	}
	
	//TEST (grow)
	//{
		//BMemArray arr;
		
		//uint32 old_capacity = arr.*member<capacity>::value;
		//uint32 new_capacity;
		
		//(arr.*func<grow>::ptr)();
		
		//new_capacity = arr.*member<capacity>::value;
		
		//CHECK(old_capacity < new_capacity);
	//}
}
