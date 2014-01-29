#ifndef PROPERTY_H
#define PROPERTY_H

#include <cstdarg>

#include "public.h"

OPEN_O2_NAMESPACE

template<typename _class, typename _type>
class cProperty
{
	_class* mClass;
	
	void (_class::*mSetter)(const _type&);
	_type (_class::*mGetter)();

public:
	cProperty():mSetter(NULL), mGetter(NULL), mClass(NULL) {}

	void init(_class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)())
	{
		mClass = tclass;
		mSetter = setter;
		mGetter = getter;
	}

	operator _type() 
	{
		return (mClass->*mGetter)();
	}

	_type v() 
	{
		return (mClass->*mGetter)();
	}
	
	cProperty& operator=(const _type& value)
	{
		(mClass->*mSetter)(value);
		return *this;
	}
	
	cProperty& operator+=(const _type& value)
	{
		*this = *this + value;
		return *this;
	}

	_type operator+(const _type& value)
	{
		return (mClass->*mGetter)() + value;
	}
	
	cProperty& operator-=(const _type& value)
	{
		*this = *this - value;
		return *this;
	}

	_type operator-(const _type& value)
	{
		return (mClass->*mGetter)() - value;
	}
	
	cProperty& operator*=(const _type& value)
	{
		*this = *this * value;
		return *this;
	}

	_type operator*(const _type& value)
	{
		return (mClass->*mGetter)() * value;
	}
	
	cProperty& operator/=(const _type& value)
	{
		*this = *this / value;
		return *this;
	}

	_type operator/(const _type& value)
	{
		return (mClass->*mGetter)() / value;
	}

protected:
	cProperty& operator=(const cProperty& prop) { return *this; }
};

#define PROPERTY(_CLASS, _TYPE) cProperty<_CLASS, _TYPE>

CLOSE_O2_NAMESPACE

#endif //PROPERTY_H