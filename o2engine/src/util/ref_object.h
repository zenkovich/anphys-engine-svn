#ifndef REF_COUNTER_H
#define REF_COUNTER_H

#include "public.h"

OPEN_O2_NAMESPACE

/** Object, containing reference count. */
class IRefObject
{
protected:
	int mRefCount; /**< Reference count. */

public:
	/** ctor. */
	IRefObject():mRefCount(0) {}

	/** dtor. */
	virtual ~IRefObject() {}

	/** Increase reference count. */
	void incRefCount() 
	{
		mRefCount++;
		onRefIncreased();
	}

	/** Decrease reference count. */
	void decRefCount() 
	{
		mRefCount--;
		onRefDecreased();
		if (mRefCount == 0)
			onZeroRefCount();
	}

	/** Returns reference count. */
	int getRefCount()
	{
		return mRefCount;
	}

protected:
	/** Runs when reference counter was increased. */
	virtual void onRefIncreased() {}

	/** Runs when reference counter was decraised.*/
	virtual void onRefDecreased() {}

	/** Runs when reference counter sets to zero. */
	virtual void onZeroRefCount() {}
};


/** Reference object. Containing IRefObject and working with reference count. */
template<typename T>
class cObjectRef
{
protected:
	T* mObject; /**< IRefObject object. */

public:
	/** ctor. */
	cObjectRef(T* object):mObject(object)
	{
		if (mObject)
			mObject->incRefCount();
	}

	/** copy ctor. Increases reference. */
	cObjectRef(const cObjectRef<T>& refObject)
	{
		mObject = refObject.mObject;
		if (mObject)
			mObject->incRefCount();
	}

	/** dtor. */
	virtual ~cObjectRef() 
	{
		if (mObject)
			mObject->decRefCount();
	}

	/** copy operator. */
	cObjectRef<T> operator=(const cObjectRef<T>& refObject)
	{
		mObject = refObject.mObject;
		if (mObject)
			mObject->incRefCount();
		return *this;
	}
};

#define DEFINE_REF_TYPE() public:                                        \
	virtual UniqueType getType() { return mObject->getType(); }          \
	virtual const char* getTypeName() { return mObject->getTypeName(); }

CLOSE_O2_NAMESPACE

#endif // REF_COUNTER_H
