/* Simple but low-efficiency key-value map.	*/

#ifndef BDICTIONARY_H_
#define BDICTIONARY_H_

#include <osndef.h>

struct BDictEntry;

class BDict
{
private:
	BDictEntry *entries;
	uint32 allocated, used;
	
public:
	BDict(uint32 init_size = 8);
	~BDict();
	
	void   add(const char *name, void *value);
	bool   set(const char *name, void *value);	// Sets the value of an existing key. Returns false if the key was not found.
	void  *get(const char *name);

	void  foreach(void (*iter_func)(const char *key, void *value));
	void  foreach(void (*iter_func)(const char *key, void *value, void *data), void *data);	// Custom data can be passed using the `data` argument.

private:
	BDictEntry *grow();		// Grows the array and returns a pointer to the first new free pair.
	void  shrink();
	
	BDictEntry *new_entry();		// Used to request a new entry. Either finds a free existing pair or grows the array if none are found.
	BDictEntry *find_entry(const char *name);
};

struct BDictEntry
{
	char *name;		// If the `name` pointer is null, the entry is free to be overwritten.
	void *data;

	static BDictEntry  *next_free(BDictEntry *pairs, uint32 allocated);		// Gets the next free pair or returns NULL if all pairs are used.
	static uint32 count_free(BDictEntry *pairs, uint32 allocated);
	
	static void   mark_free(BDictEntry *pair);
};
// set: overwrites or creates
// set_..: creates but does not overwrite
#endif
