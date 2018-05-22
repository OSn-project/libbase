#ifndef __BASE_ARRAY_H__
#define __BASE_ARRAY_H__

#include 

class BArrayMaintainer
{
	size_t item_sz;
	
	uint32 length;
	uint32 capacity;
	
	uint8 *data;

public:
	BArrayMaintainer(size_t item_sz, );
	~BArrayMaintainer();
	
private:
	void grow();
}

template <typename T>
class BArray : public BArrayMaintainer
{
public:
	BArray() : BArrayMaintainer(sizeof(T));
}

#endif
