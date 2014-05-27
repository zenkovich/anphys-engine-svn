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
	void (_class::*mSetterNonConst)(_type); /** Setter function pointer for non const parameter */
	_type (_class::*mGetter)() const;       /** Getter function pointer. */
	_type (_class::*mGetterNonConst)();     /** Non const getter function pointer. */

public:
	/** ctor. */
	cProperty():mSetter(NULL), mGetter(NULL), mClass(NULL) {}

	/** Initialization of property. */
	void init(_class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)() const)
	{
		mClass = tclass;
		mSetter = setter;
		mSetterNonConst = NULL;
		mGetter = getter;
		mGetterNonConst = NULL;
	}

	/** Initialization of property. */
	void initNonConstGetter(_class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)())
	{
		mClass = tclass;
		mSetter = setter;
		mSetterNonConst = NULL;
		mGetter = NULL;
		mGetterNonConst = getter;
	}

	/** Initialization of property. */
	void initNonConstSetter(_class* tclass, void (_class::*setter)(_type), _type (_class::*getter)() const)
	{
		mClass = tclass;
		mSetter = NULL;
		mSetterNonConst = setter;
		mGetter = getter;
		mGetterNonConst = NULL;
	}

	/** Initialization of property. */
	void initNonConst(_class* tclass, void (_class::*setter)(_type), _type (_class::*getter)())
	{
		mClass = tclass;
		mSetter = NULL;
		mSetterNonConst = setter;
		mGetter = NULL;
		mGetterNonConst = getter;
	}

	/** Type conversion operator. */
	operator _type() 
	{
		return get();
	}

	/** Returns value. */
	_type get() 
	{
		return mGetter != NULL ? (mClass->*mGetter)():(mClass->*mGetterNonConst)();
	}

	void set(const _type& value)
	{
		if (mSetter != NULL)
			(mClass->*mSetter)(value);
		else
			(mClass->*mSetterNonConst)(value);
	}
	
	cProperty& operator=(const _type& value)
	{
		set(value);
		return *this;
	}
	
	cProperty& operator+=(const _type& value)
	{
		*this = *this + value;
		return *this;
	}

	_type operator+(const _type& value)
	{
		return get() + value;
	}
	
	cProperty& operator-=(const _type& value)
	{
		*this = *this - value;
		return *this;
	}

	_type operator-(const _type& value)
	{
		return get() - value;
	}
	
	cProperty& operator*=(const _type& value)
	{
		*this = *this * value;
		return *this;
	}

	_type operator*(const _type& value)
	{
		return get() * value;
	}
	
	cProperty& operator/=(const _type& value)
	{
		*this = *this / value;
		return *this;
	}

	_type operator/(const _type& value)
	{
		return get() / value;
	}

protected:
	/** Please, don't copy this. */
	cProperty& operator=(const cProperty& prop) { return *this; }
};

/** Simple macros for highlighting. */
#define PROPERTY(_CLASS, _TYPE) cProperty<_CLASS, _TYPE>

CLOSE_O2_NAMESPACE

#endif //PROPERTY_H