#ifndef PROPERTY_H
#define PROPERTY_H

#include <cstdarg>

#include "public.h"
#include "util/property_list.h"

OPEN_O2_NAMESPACE


/** Super-duper-mega cpp property! Hello from C#. I think not bad feature :). */
template<typename _class, typename _type>
class cProperty: public cPropertyList::Property<_type>
{
	_class* mClass;                         /** Basic class. */
	
	void (_class::*mSetter)(const _type&);  /** Awww yeaaahh it's setter function pointer %) */
	void (_class::*mSetterNonConst)(_type); /** Setter function pointer for non const parameter */
	_type (_class::*mGetter)() const;       /** Getter function pointer. */
	_type (_class::*mGetterNonConst)();     /** Non const getter function pointer. */


public:
	/** ctor. */
	cProperty();

	/* ctor. */
	cProperty(const string& name, _class* tclass, void* setterFunc, void* getterFunc, bool setterConst = true, bool getterConst = true);

	/** Initialization of property. */
	void init(const string& name, _class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)() const);

	/** Initialization of property. */
	void initNonConstGetter(const string& name, _class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)());

	/** Initialization of property. */
	void initNonConstSetter(const string& name, _class* tclass, void (_class::*setter)(_type), _type (_class::*getter)() const);

	/** Initialization of property. */
	void initNonConst(const string& name, _class* tclass, void (_class::*setter)(_type), _type (_class::*getter)());

	/** Type conversion operator. */
	operator _type();

	/** Returns value. */
	_type get() const;

	/** Sets value. */
	void set(const _type& value);

	/** Copying property. */
	void copy(cProperty& prop) const;

	cProperty& operator=(const _type& value);	
	cProperty& operator+=(const _type& value);
	_type      operator+(const _type& value);	
	cProperty& operator-=(const _type& value);
	_type      operator-(const _type& value);	
	cProperty& operator*=(const _type& value);
	_type      operator*(const _type& value);	
	cProperty& operator/=(const _type& value);
	_type      operator/(const _type& value);

protected:
	/** Please, don't copy this. */
	cProperty& operator=(const cProperty& prop);
};

//implementation
template<typename _class, typename _type>
cProperty<_class, _type>::cProperty(): mSetter(NULL), mGetter(NULL), mClass(NULL) {}

template<typename _class, typename _type>
cProperty<_class, _type>::cProperty(const string& name, _class* tclass, void* setterFunc, void* getterFunc, bool setterConst = true, bool getterConst = true)
{
	mName = name;
	mClass = tclass;
	mSetter = setterConst ? NULL:setterFunc;
	mSetterNonConst = setterConst ? setterFunc:NULL;
	mGetter = getterConst ? NULL:getterFunc;
	mGetterNonConst = getterConst ? getterFunc:NULL;
	checkPropertyList(tclass);
}

template<typename _class, typename _type>
void cProperty<_class, _type>::init(const string& name, _class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)() const)
{
	mName = name;
	mClass = tclass;
	mSetter = setter;
	mSetterNonConst = NULL;
	mGetter = getter;
	mGetterNonConst = NULL;
	checkPropertyList(tclass);
}

template<typename _class, typename _type>
void cProperty<_class, _type>::initNonConstGetter(const string& name, _class* tclass, void (_class::*setter)(const _type&), _type (_class::*getter)())
{
	mName = name;
	mClass = tclass;
	mSetter = setter;
	mSetterNonConst = NULL;
	mGetter = NULL;
	mGetterNonConst = getter;
	checkPropertyList(tclass);
}

template<typename _class, typename _type>
void cProperty<_class, _type>::initNonConstSetter(const string& name, _class* tclass, void (_class::*setter)(_type), _type (_class::*getter)() const)
{
	mName = name;
	mClass = tclass;
	mSetter = NULL;
	mSetterNonConst = setter;
	mGetter = getter;
	mGetterNonConst = NULL;
	checkPropertyList(tclass);
}

template<typename _class, typename _type>
void cProperty<_class, _type>::initNonConst(const string& name, _class* tclass, void (_class::*setter)(_type), _type (_class::*getter)())
{
	mName = name;
	mClass = tclass;
	mSetter = NULL;
	mSetterNonConst = setter;
	mGetter = NULL;
	mGetterNonConst = getter;
	checkPropertyList(tclass);
}

template<typename _class, typename _type>
cProperty<_class, _type>::operator _type() 
{
	return get();
}

template<typename _class, typename _type>
_type cProperty<_class, _type>::get() const
{
	return mGetter != NULL ? (mClass->*mGetter)():(mClass->*mGetterNonConst)();
}

template<typename _class, typename _type>
void cProperty<_class, _type>::set(const _type& value)
{
	if (mSetter != NULL)
		(mClass->*mSetter)(value);
	else
		(mClass->*mSetterNonConst)(value);

	onChangeEvent.call();
}

template<typename _class, typename _type>
void cProperty<_class, _type>::copy(cProperty& prop) const
{
	prop.mClass = mClass;
	prop.mSetterNonConst = mSetterNonConst;
	prop.mSetter = mSetter;
	prop.mGetter = mGetter;
	prop.mGetterNonConst = mGetterNonConst;
}

template<typename _class, typename _type>
cProperty<_class, _type>& cProperty<_class, _type>::operator=(const _type& value)
{
	set(value);
	return *this;
}
	
template<typename _class, typename _type>
cProperty<_class, _type>& cProperty<_class, _type>::operator+=(const _type& value)
{
	*this = *this + value;
	return *this;
}

template<typename _class, typename _type>
_type cProperty<_class, _type>::operator+(const _type& value)
{
	return get() + value;
}
	
template<typename _class, typename _type>
cProperty<_class, _type>& cProperty<_class, _type>::operator-=(const _type& value)
{
	*this = *this - value;
	return *this;
}

template<typename _class, typename _type>
_type cProperty<_class, _type>::operator-(const _type& value)
{
	return get() - value;
}
	
template<typename _class, typename _type>
cProperty<_class, _type>& cProperty<_class, _type>::operator*=(const _type& value)
{
	*this = *this * value;
	return *this;
}

template<typename _class, typename _type>
_type cProperty<_class, _type>::operator*(const _type& value)
{
	return get() * value;
}
	
template<typename _class, typename _type>
cProperty<_class, _type>& cProperty<_class, _type>::operator/=(const _type& value)
{
	*this = *this / value;
	return *this;
}

template<typename _class, typename _type>
_type cProperty<_class, _type>::operator/(const _type& value)
{
	return get() / value;
}

template<typename _class, typename _type>
cProperty<_class, _type>& cProperty<_class, _type>::operator=(const cProperty& prop) 
{ 
	mClass = prop.mClass;
	mSetterNonConst = prop.mSetterNonConst;
	mSetter = prop.mSetter;
	mGetter = prop.mGetter;
	mGetterNonConst = prop.mGetterNonConst;

	return *this; 
}

/** Simple macros for highlighting. */
#define PROPERTY(_CLASS, _TYPE) cProperty<_CLASS, _TYPE>

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