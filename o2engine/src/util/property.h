#ifndef PROPERTY_H
#define PROPERTY_H

#include <cstdarg>

#include "public.h"

OPEN_O2_NAMESPACE

/** Super-duper-mega cpp property! Hello from C#. I think not bad feature :). */
template<typename _class, typename _type>
class cProperty
{
	_class* mClass; /** Basic class. */
	
	void (_class::*mSetter)(const _type&);  /** Awww yeaaahh it's setter function pointer %) */
	_type (_class::*mGetter)() const;       /** Getter function pointer. */

public:
	/** ctor. */
	cProperty():mSetter(NULL), mGetter(NULL), mClass(NULL) {}

	/** Initialization of property. */
	void init(_class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)() const)
	{
		mClass = tclass;
		mSetter = setter;
		mGetter = getter;
	}

	/** Type convertion operator. */
	operator _type() 
	{
		return (mClass->*mGetter)();
	}

	/** Returns value. */
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
	/** Please, don't copy this. */
	cProperty& operator=(const cProperty& prop) { return *this; }
};

/** Simple macros for hightlighting. */
#define PROPERTY(_CLASS, _TYPE) cProperty<_CLASS, _TYPE>

CLOSE_O2_NAMESPACE

#endif //PROPERTY_H