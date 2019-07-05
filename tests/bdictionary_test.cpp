#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "privablic.h"

#include <base/dict.h>
#include <base/misc.h>

/* Private members */
struct DICTIONARY_entries { typedef BDictEntry *(BDict::*type); };
template class private_member<DICTIONARY_entries, &BDict::entries>;

struct DICTIONARY_allocated { typedef uint32 (BDict::*type); };
template class private_member<DICTIONARY_allocated, &BDict::allocated>;

struct DICTIONARY_used { typedef uint32 (BDict::*type); };
template class private_member<DICTIONARY_used, &BDict::used>;

/* Private methods */
struct DICTIONARY_grow { typedef BDictEntry *(BDict::*type)(); };
template class private_method<DICTIONARY_grow, &BDict::grow>;

struct DICTIONARY_new_entry { typedef BDictEntry *(BDict::*type)(); };
template class private_method<DICTIONARY_new_entry, &BDict::new_entry>;

SUITE(BDict)
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
		memcpy(out_mem, in_mem, sizeof(BDictEntry));
		return out_mem;
	}
	
	TEST (ConstructorDestructor)
	{
		MockRepository mocks;
		mocks.autoExpect = false;
    
		mocks.OnCallFunc(malloc).Do(dirty_malloc);	// If the pairs array is malloced instead of calloced, return dirty memory to make sure that the pairs are still marked as free.
		
		BDict *dict = new BDict(1);
		
		CHECK(dict->*member<DICTIONARY_allocated>::value == 1);
		CHECK(dict->*member<DICTIONARY_used>::value == 0);
		CHECK((dict->*member<DICTIONARY_entries>::value)[0].name == NULL);		// Check that the pair has been marked as free
		
		/* Destructor */
		mocks.ExpectCallFunc(free).With(dict->*member<DICTIONARY_entries>::value);
		
		delete dict;
	}

	TEST (add)
	{
		BDict dict(1);
		
		dict.add("key", (void *) 0x76616c75);
		
		CHECK(streq((dict.*member<DICTIONARY_entries>::value)[0].name, "key"));
		CHECK((dict.*member<DICTIONARY_entries>::value)[0].data == (void *) 0x76616c75);
		CHECK(dict.*member<DICTIONARY_used>::value == 1);
	}
	
	TEST (set)
	{
		BDict dict;
		dict.add("one", (void *) 1);
		dict.add("two", (void *) 2);
		dict.add("three", (void *) 3);
		
		{
			bool rc = dict.set("two", (void *) 5);
			
			CHECK(rc == true);
			CHECK(dict.get("two") == (void *) 5);
		}
		{
			bool rc = dict.set("four", (void *) 8);
			
			CHECK(rc == false);
		}
	}

	TEST (get)
	{
		BDict dict;
		dict.add("one", (void *) 1);
		dict.add("two", (void *) 2);
		dict.add("three", (void *) 3);
		
		CHECK(dict.get("two") == (void *) 2);
		CHECK(dict.get("five") == NULL);
	}
	
	TEST (grow)
	{
		MockRepository mocks;
		BDict dict(1);		// We test growing from 1 because (1*3)/2 = 1
		
		uint32 old_allocated = dict.*member<DICTIONARY_allocated>::value;
		uint32 new_allocated;
		
		mocks.OnCallFunc(realloc).Do(dirty_realloc);
		
		BDictEntry *new_entrys = (&dict->*func<DICTIONARY_grow>::ptr)();
		
		new_allocated = dict.*member<DICTIONARY_allocated>::value;
		
		CHECK(old_allocated < new_allocated);
		CHECK(new_entrys[0].name == NULL);
	}
	
	TEST (new_entry)
	{
		MockRepository mocks;
		
		/* Existing free pairs */
		{
			BDict dict1(2);		// We test growing from 1 because (1*3)/2 = 1
			
			BDictEntry *pair = (&dict1->*func<DICTIONARY_new_entry>::ptr)();
			
			CHECK(	pair == &(dict1.*member<DICTIONARY_entries>::value)[0] ||
					pair == &(dict1.*member<DICTIONARY_entries>::value)[1]);
		}
		
		/* No free pairs */
		{
			BDict dict2(4);		// We test growing from 1 because (1*3)/2 = 1
			dict2.*member<DICTIONARY_used>::value = 4;	// Pretend that all pairs are used
			memset(dict2.*member<DICTIONARY_entries>::value, 0xff, 4 * sizeof(BDictEntry));
			
			//mocks.ExpectCall(&dict2, BDict::grow);	// Oh well...
			BDictEntry *pair = (&dict2->*func<DICTIONARY_new_entry>::ptr)();
			
			CHECK(pair->name == NULL);
		}
	}

	TEST (Entry_next_free)
	{
		BDictEntry pairs[] = {
			{"key1", NULL},
			{"key2", (void *) 0x1234},
			{NULL,   (void *) 0x5678},
			{"key3", (void *) 0x9abc}
		};
		
		CHECK (BDictEntry::next_free(pairs, 2) == NULL);
		CHECK (BDictEntry::next_free(pairs, 4) == &pairs[2]);
	}

	TEST (Entry_mark_free)
	{
		BDictEntry pair = {(char *) 0x1234, (void *) 0x5678};
		
		BDictEntry::mark_free(&pair);
		
		CHECK (pair.name == NULL);
	}
}
