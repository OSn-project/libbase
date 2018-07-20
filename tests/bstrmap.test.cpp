#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "privablic.h"

#include <base/strmap.h>
#include <base/misc.h>

/* Private members */
struct STRMAP_pairs { typedef BStrMap::Pair *(BStrMap::*type); };
template class private_member<STRMAP_pairs, &BStrMap::pairs>;

struct STRMAP_allocated { typedef uint32 (BStrMap::*type); };
template class private_member<STRMAP_allocated, &BStrMap::allocated>;

struct STRMAP_used { typedef uint32 (BStrMap::*type); };
template class private_member<STRMAP_used, &BStrMap::used>;

/* Private methods */
struct STRMAP_grow { typedef BStrMap::Pair *(BStrMap::*type)(); };
template class private_method<STRMAP_grow, &BStrMap::grow>;

struct STRMAP_new_pair { typedef BStrMap::Pair *(BStrMap::*type)(); };
template class private_method<STRMAP_new_pair, &BStrMap::new_pair>;

SUITE(BStrMap)
{
	void *dirty_malloc(size_t size)
	{
		/* Returns malloced memory that is guaranteed not to be zeros */
		void *mem = malloc(size);
		memset(mem, 0xff, size);
		return mem;
	}

	void *dirty_realloc(void *in_mem, size_t size)
	{
		/* ONLY TO BE USED BY THE `grow` TEST WITH AN ORIGINAL SIZE OF 1 PAIR.	*
		 * This only copies the first pair into the new memory.					*/
		void *out_mem = malloc(size);
		memset(out_mem, 0xff, size);
		memcpy(out_mem, in_mem, sizeof(BStrMap::Pair));
		return out_mem;
	}
	
	TEST (ConstructorDestructor)
	{
		MockRepository mocks;
		mocks.autoExpect = false;
    
		mocks.OnCallFunc(malloc).Do(dirty_malloc);	// If the pairs array is malloced instead of calloced, return dirty memory to make sure that the pairs are still marked as free.
		
		BStrMap *map = new BStrMap(1);
		
		CHECK(map->*member<STRMAP_allocated>::value == 1);
		CHECK(map->*member<STRMAP_used>::value == 0);
		CHECK((map->*member<STRMAP_pairs>::value)[0].name == NULL);		// Check that the pair has been marked as free
		
		/* Destructor */
		mocks.ExpectCallFunc(free).With(map->*member<STRMAP_pairs>::value);
		
		delete map;
	}

	TEST (add)
	{
		BStrMap map(1);
		
		map.add("key", (void *) 0x76616c75);
		
		CHECK(streq((map.*member<STRMAP_pairs>::value)[0].name, "key"));
		CHECK((map.*member<STRMAP_pairs>::value)[0].data == (void *) 0x76616c75);
		CHECK(map.*member<STRMAP_used>::value == 1);
	}
	
	TEST (set)
	{
		BStrMap map;
		map.add("one", (void *) 1);
		map.add("two", (void *) 2);
		map.add("three", (void *) 3);
		
		{
			bool rc = map.set("two", (void *) 5);
			
			CHECK(rc == true);
			CHECK(map.get("two") == (void *) 5);
		}
		{
			bool rc = map.set("four", (void *) 8);
			
			CHECK(rc == false);
		}
	}

	TEST (get)
	{
		BStrMap map;
		map.add("one", (void *) 1);
		map.add("two", (void *) 2);
		map.add("three", (void *) 3);
		
		CHECK(map.get("two") == (void *) 2);
		CHECK(map.get("five") == NULL);
	}
	
	TEST (grow)
	{
		MockRepository mocks;
		BStrMap map(1);		// We test growing from 1 because (1*3)/2 = 1
		
		uint32 old_allocated = map.*member<STRMAP_allocated>::value;
		uint32 new_allocated;
		
		mocks.OnCallFunc(realloc).Do(dirty_realloc);
		
		BStrMap::Pair *new_pairs = (&map->*func<STRMAP_grow>::ptr)();
		
		new_allocated = map.*member<STRMAP_allocated>::value;
		
		CHECK(old_allocated < new_allocated);
		CHECK(new_pairs[0].name == NULL);
	}
	
	TEST (new_pair)
	{
		MockRepository mocks;
		
		/* Existing free pairs */
		{
			BStrMap map1(2);		// We test growing from 1 because (1*3)/2 = 1
			
			BStrMap::Pair *pair = (&map1->*func<STRMAP_new_pair>::ptr)();
			
			CHECK(	pair == &(map1.*member<STRMAP_pairs>::value)[0] ||
					pair == &(map1.*member<STRMAP_pairs>::value)[1]);
		}
		
		/* No free pairs */
		{
			BStrMap map2(4);		// We test growing from 1 because (1*3)/2 = 1
			map2.*member<STRMAP_used>::value = 4;	// Pretend that all pairs are used
			memset(map2.*member<STRMAP_pairs>::value, 0xff, 4 * sizeof(BStrMap::Pair));	
			
			//mocks.ExpectCall(&map2, BStrMap::grow);	// Oh well...
			BStrMap::Pair *pair = (&map2->*func<STRMAP_new_pair>::ptr)();
			
			CHECK(pair->name == NULL);
		}
	}

	TEST (Pair_next_free)
	{
		BStrMap::Pair pairs[] = {
			{"key1", NULL},
			{"key2", (void *) 0x1234},
			{NULL,   (void *) 0x5678},
			{"key3", (void *) 0x9abc}
		};
		
		CHECK (BStrMap::Pair::next_free(pairs, 2) == NULL);
		CHECK (BStrMap::Pair::next_free(pairs, 4) == &pairs[2]);
	}

	TEST (Pair_mark_free)
	{
		BStrMap::Pair pair = {(char *) 0x1234, (void *) 0x5678};
		
		BStrMap::Pair::mark_free(&pair);
		
		CHECK (pair.name == NULL);
	}
}
