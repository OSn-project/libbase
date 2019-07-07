/* Simple but low-efficiency key-value map.	*/

#ifndef BDICTIONARY_H_
#define BDICTIONARY_H_

#include <osndef.h>

struct BDictEntry
{
	char *name;		// If the `name` pointer is null, the entry is free to be overwritten.
	void *data;

	static BDictEntry  *next_free(BDictEntry *pairs, uint32 allocated);		// Gets the next free pair or returns NULL if all pairs are used.
	static uint32 count_free(BDictEntry *pairs, uint32 allocated);

	static void   mark_free(BDictEntry *pair);
};

class BBaseDict
{
protected:
	BDictEntry *entries;
	uint32 allocated, used;
	
public:
	BBaseDict(uint32 init_size = 8);
	~BBaseDict();
	
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

template <typename T> class BDict;

template <typename T>
class BDict<T *> : public BBaseDict		// <- means we can only store pointer types in the dictionary
{
public:
	inline BDict() : BBaseDict() {}

	inline void add(const char *name, T *value) { this->BBaseDict::add(name, (void *) value); }
	inline bool set(const char *name, T *value) { return this->BBaseDict::set(name, (void *) value); }
	inline T   *get(const char *name)           { return (T *) this->BBaseDict::get(name); }

	inline void foreach(void (*iter_func)(const char *key, T *value))
	{
		this->BBaseDict::foreach((void (*)(const char *, void *)) iter_func);
	}

	template <typename C>
	inline void foreach(void (*iter_func)(const char *key, T *value, C *data), C data)	// Custom data can be passed using the `data` argument. Remember C must be size of a pointer.
	{
		this->BBaseDict::foreach((void (*)(const char *, void *, void *)) iter_func, (void *) data);
	}
};

// set: overwrites or creates
// safe_set..: creates but does not overwrite
#endif
