/*
 * stack.h
 *
 *  Created on: Dec 14, 2018
 *      Author: albert
 */

#ifndef BSTACK_H_
#define BSTACK_H_

#include "array.h"

template <typename T>
class BStack : private BArray<T>
{
public:
	BStack() : BArray<T>()
	{
	}

	inline void push(T item)
	{
		this->BArray<T>::add(item);
	}

	inline T pop()
	{
		T ret = this->BArray<T>::get(this->m_len - 1);
		this->BArray<T>::remove(this->m_len - 1);

		return ret;
	}

	inline T peek(int32 index)
	{
		if (index < 0) index += this->m_len;
		return this->BArray<T>::get(index);
	}

	inline uint32 length()
	{
		return this->m_len;
	}
};

#endif
