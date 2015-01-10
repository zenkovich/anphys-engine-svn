#ifndef PROPERTY_H
#define PROPERTY_H

#include <cstdarg>

#include "public.h"
#include "util/property_list.h"

OPEN_O2_NAMESPACE


/** Super-duper-mega cpp property! Hello from C#. I think not bad feature :). */
template<typename _class, typename _type>
class Property: public PropertyList::Property<_type>
{
	_class* mClass;                         /** Basic class. */
	
	void (_class::*mSetter)(const _type&);  /** Awww yeaaahh it's setter function pointer %) */
	void (_class::*mSetterNonConst)(_type); /** Setter function pointer for non const parameter */
	_type (_class::*mGetter)() const;       /** Getter function pointer. */
	_type (_class::*mGetterNonConst)();     /** Non const getter function pointer. */


public:
	/** ctor. */
	Property(): mSetter(NULL), mGetter(NULL), mClass(NULL) {}

	Property(const string& name, _class* tclass, void* setterFunc, void* getterFunc, bool setterConst = true, bool getterConst = true)
	{
		mName = name;
		mClass = tclass;
		mSetter = setterConst ? NULL:setterFunc;
		mSetterNonConst = setterConst ? setterFunc:NULL;
		mGetter = getterConst ? NULL:getterFunc;
		mGetterNonConst = getterConst ? getterFunc:NULL;
		checkPropertyList(tclass);
	}

	/** Initialization of property. */
	void init(const string& name, _class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)() const)
	{
		mName = name;
		mClass = tclass;
		mSetter = setter;
		mSetterNonConst = NULL;
		mGetter = getter;
		mGetterNonConst = NULL;
		checkPropertyList(tclass);
	}

	/** Initialization of property. */
	void initNonConstGetter(const string& name, _class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)())
	{
		mName = name;
		mClass = tclass;
		mSetter = setter;
		mSetterNonConst = NULL;
		mGetter = NULL;
		mGetterNonConst = getter;
		checkPropertyList(tclass);
	}

	/** Initialization of property. */
	void initNonConstSetter(const string& name, _class* tclass, void (_class::*setter)(_type), _type (_class::*getter)() const)
	{
		mName = name;
		mClass = tclass;
		mSetter = NULL;
		mSetterNonConst = setter;
		mGetter = getter;
		mGetterNonConst = NULL;
		checkPropertyList(tclass);
	}

	/** Initialization of property. */
	void initNonConst(const string& name, _class* tclass, void (_class::*setter)(_type), _type (_class::*getter)())
	{
		mName = name;
		mClass = tclass;
		mSetter = NULL;
		mSetterNonConst = setter;
		mGetter = NULL;
		mGetterNonConst = getter;
		checkPropertyList(tclass);
	}

	/** Type conversion operator. */
	operator _type() 
	{
		return get();
	}

	/** Returns value. */
	_type get() const
	{
		return mGetter != NULL ? (mClass->*mGetter)():(mClass->*mGetterNonConst)();
	}

	void set(const _type& value)
	{
		if (mSetter != NULL)
			(mClass->*mSetter)(value);
		else
			(mClass->*mSetterNonConst)(value);

		onChangeEvent.call();
	}

	void copy(Property& prop) const
	{
		prop.mClass = mClass;
		prop.mSetterNonConst = mSetterNonConst;
		prop.mSetter = mSetter;
		prop.mGetter = mGetter;
		prop.mGetterNonConst = mGetterNonConst;
	}

	Property& operator=(const _type& value)
	{
		set(value);
		return *this;
	}
	
	Property& operator+=(const _type& value)
	{
		*this = *this + value;
		return *this;
	}

	_type operator+(const _type& value)
	{
		return get() + value;
	}
	
	Property& operator-=(const _type& value)
	{
		*this = *this - value;
		return *this;
	}

	_type operator-(const _type& value)
	{
		return get() - value;
	}
	
	Property& operator*=(const _type& value)
	{
		*this = *this * value;
		return *this;
	}

	_type operator*(const _type& value)
	{
		return get() * value;
	}
	
	Property& operator/=(const _type& value)
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
	Property& operator=(const Property& prop) 
	{ 
		mClass = prop.mClass;
		mSetterNonConst = prop.mSetterNonConst;
		mSetter = prop.mSetter;
		mGetter = prop.mGetter;
		mGetterNonConst = prop.mGetterNonConst;

		return *this; 
	}
};

/** Simple macros for highlighting. */
#define PROPERTY(_CLASS, _TYPE) Property<_CLASS, _TYPE>

#define REG_PROPERTY(_CLASS, _PROPERTY, _SETTER, _GETTER) \
	_PROPERTY.init(#_PROPERTY, this, &_CLASS::_SETTER, &_CLASS::_GETTER)

#define REG_PROPERTY_SETTER_NONCONST(_CLASS, _PROPERTY, _SETTER, _GETTER) \
	_PROPERTY.initNonConstSetter(#_PROPERTY, this, &_CLASS::_SETTER, &_CLASS::_GETTER)

#define REG_PROPERTY_GETTER_NONCONST(_CLASS, _PROPERTY, _SETTER, _GETTER) \
	_PROPERTY.initNonConstGetter(#_PROPERTY, this, &_CLASS::_SETTER, &_CLASS::_GETTER)

#define REG_PROPERTY_NONCONST(_CLASS, _PROPERTY, _SETTER, _GETTER) \
	_PROPERTY.initNonConst(#_PROPERTY, this, &_CLASS::_SETTER, &_CLASS::_GETTER)

CLOSE_O2_NAMESPACE

#endif //PROPERTY_H