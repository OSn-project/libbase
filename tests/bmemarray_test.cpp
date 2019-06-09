#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "privablic.h"

#include <base/array.h>
#include <base/misc.h>

/* Private members */
struct MEMARRAY_item_size { typedef size_t (BMemArray::*type); };
template class private_member<MEMARRAY_item_size, &BMemArray::item_size>;

struct MEMARRAY_m_len { typedef uint32 (BMemArray::*type); };
template class private_member<MEMARRAY_m_len, &BMemArray::m_len>;

struct MEMARRAY_capacity { typedef uint32 (BMemArray::*type); };
template class private_member<MEMARRAY_capacity, &BMemArray::capacity>;

struct MEMARRAY_data { typedef uint8 *(BMemArray::*type); };
template class private_member<MEMARRAY_data, &BMemArray::data>;

/* Private methods */
struct MEMARRAY_grow { typedef void (BMemArray::*type)(); };
template class private_method<MEMARRAY_grow, &BMemArray::grow>;

SUITE(BMemArray)
{
	TEST (Constructor)
	{
		MockRepository mocks;
		
		mocks.ExpectCallFunc(malloc).Return((void *) 0x1234);
		
		BMemArray *arr = new BMemArray(3);
		
		CHECK(arr->*member<MEMARRAY_item_size>::value == 3);		
		CHECK(arr->*member<MEMARRAY_m_len>::value == 0);
		
		CHECK(arr->*member<MEMARRAY_data>::value == (uint8 *) 0x1234);
		
		mocks.ExpectCallFunc(free).With((void *) 0x1234);
		delete arr;
	}
	
	TEST (add)
	{
		BMemArray arr(sizeof(uint16));
		
		uint16 test_data[] = {15, 12000};
		
		arr.add(&test_data[0]);	// Give it the address of the value we want to copy
		CHECK(arr.*member<MEMARRAY_m_len>::value == 1);

		arr.add(&test_data[1]);
		CHECK(arr.*member<MEMARRAY_m_len>::value == 2);
		
		CHECK(memcmp(test_data, arr.*member<MEMARRAY_data>::value, sizeof(test_data)) == 0);
	}
	
	TEST (remove)
	{
		int32 test_data[] = {0, 1, 2};
		BMemArray *arr = BMemArray::from_static(&test_data, 3, sizeof(int32));

		arr->remove(1);
		
		CHECK(arr->length() == 2);
		CHECK(*((uint32 *) arr->get_ptr(1)) == 2);
		
		delete arr;
	}
	
	TEST (from_static)
	{
		/*REQUIRE*/ {
			int32 test_data[] = {0, 1, 2, 3};
			BMemArray *arr = BMemArray::from_static(&test_data, 4, sizeof(int32));
			
			CHECK(arr->length() == 4);

			CHECK(*((uint32 *) arr->get_ptr(0)) == 0);
			CHECK(*((uint32 *) arr->get_ptr(1)) == 1);
			CHECK(*((uint32 *) arr->get_ptr(2)) == 2);
			CHECK(*((uint32 *) arr->get_ptr(3)) == 3);
			
			delete arr;
		}
	}
	
	TEST (grow)
	{
		BMemArray arr(sizeof(void *));
		
		uint32 old_MEMARRAY_capacity = arr.*member<MEMARRAY_capacity>::value;
		uint32 new_MEMARRAY_capacity;
		
		(arr.*func<MEMARRAY_grow>::ptr)();
		
		new_MEMARRAY_capacity = arr.*member<MEMARRAY_capacity>::value;
		
		CHECK(old_MEMARRAY_capacity < new_MEMARRAY_capacity);
	}

	//TEST (MEMARRAY_grow_GrowsFromOne)		// (1 * 3) / 2 => 1, meaning that thearray wouldn't grow. 
	//{
		//BMemArray arr;
		
		//uint32 old_MEMARRAY_capacity = arr.*member<MEMARRAY_capacity>::value;
		//uint32 new_MEMARRAY_capacity;
		
		//(arr.*func<MEMARRAY_grow>::ptr)();
		
		//new_MEMARRAY_capacity = arr.*member<MEMARRAY_capacity>::value;
		
		//CHECK(old_MEMARRAY_capacity < new_MEMARRAY_capacity);
	//}
}

SUITE (BArray)
{
	TEST (add)
	{
		BArray<short> int_array;
		int_array.add(5);
		int_array.add(30000);
		
		CHECK(((short *) int_array.get_data())[0] == 5);
		CHECK(((short *) int_array.get_data())[1] == 30000);
	}
	
	TEST (remove_NoDestructor)
	{
		BArray<char> arr;
		
		arr.add('a');
		arr.add('b');
		arr.add('c');
		arr.add('d');
		
		arr.remove(0);
		arr.remove(1);
		
		CHECK(strncmp((const char *) (arr.*member<MEMARRAY_data>::value), "bd", 2) == 0);
	}
	
	void MOCK_free(void *mem)
	{
		return;
	}
	
	TEST (remove_WithDestructor)
	{
		struct DestroyMe
		{
			void *ptr;
			
			~DestroyMe()
			{
				MOCK_free(this->ptr);
			}
		};
		
		MockRepository mocks;
		BArray<DestroyMe> *arr = new BArray<DestroyMe>();
		
		DestroyMe *d = arr->add_new();
		d->ptr = (void *) 0x6d707472;
		
		CHECK(memcmp(arr->*member<MEMARRAY_data>::value, &d->ptr, sizeof(void *)) == 0);		// Check that the first bytes of the array's data are indeed the first bytes of the class that has been added.

		mocks.ExpectCallFunc(MOCK_free).With((void *) 0x6d707472);
		
		delete arr;
	}
	
	bool beginswithlettera(const char *item)
	{
		return item[0] == 'a';
	}

	bool beginswithletterz(const char *item)
	{
		return item[0] == 'z';
	}
	
	bool length_is(const char *item, size_t length)
	{
		return strlen(item) == length;
	}
	
	TEST (find)
	{
		const char *test_data[] = {"dandelion", "ant", "elephant", "avalanche"};
		BArray<const char *> *arr = (BArray<const char *> *) BMemArray::from_static(&test_data, 4, sizeof(char *));
		
/*		CHECK(arr->find(beginswithlettera) == &test_data[1]);
		CHECK(arr->find(beginswithletterz) == NULL);
		
		CHECK(arr->find<size_t>(length_is, 5) == &test_data[0]);
		CHECK(arr->find<const char *>(streq, "avalanche") == &test_data[3]);
		CHECK(arr->find<const char *>(streq, "xanthan") == NULL);*/
		
		delete arr;
	}
}
