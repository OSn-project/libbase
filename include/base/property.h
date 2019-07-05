/*
 * property.h
 *
 *  Created on: Dec 1, 2018
 *      Author: albert
 */

#ifndef BPROPERTY_H_
#define BPROPERTY_H_

#include <stddef.h>

template <typename T>
struct BProperty
{
	T value;

	void (*set)(BProperty<T> *prop, const T& value);
	T (*get)(BProperty<T> *prop);

	static void default_set(BProperty<T> *prop, const T& value)
	{
		prop->value = value;
	}

	static T default_get(BProperty<T> *prop)
	{
		return prop->value;
	}

	inline BProperty()
	{
		this->get = &BProperty<T>::default_get;
		this->set = &BProperty<T>::default_set;
	}

	operator= (const T &source)
	{
		this->set(this, this->value);
	}

	operator T() const
	{
		return this->get(this);
	}
};

#endif
