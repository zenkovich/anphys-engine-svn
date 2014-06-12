#ifndef REF_COUNTER_H
#define REF_COUNTER_H

#include "public.h"

OPEN_O2_NAMESPACE

/** Object, containing reference count. */
class IRefCounter
{
protected:
	int mRefCount; /**< Reference count. */

public:
	/** ctor. */
	IRefCounter():mRefCount(0) {}

	/** dtor. */
	virtual ~IRefCounter() {}

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


/** Reference object. Containing IRefCouner and working with reference count. */
template<typename T>
class cReferenceObj
{
protected:
	shared<T> mObject; /**< IRefCouner object. */

public:
	/** ctor. */
	cReferenceObj(shared<T> object):mObject(object)
	{
		if (mObject)
			mObject->incRefCount();
	}

	/** copy ctor. Increases reference. */
	cReferenceObj(const cReferenceObj<T>& refObject)
	{
		mObject = refObject.mObject;
		if (mObject)
			mObject->incRefCount();
	}

	/** dtor. */
	virtual ~cReferenceObj() 
	{
		if (mObject)
			mObject->decRefCount();
	}

	/** copy operator. */
	cReferenceObj<T> operator=(const cReferenceObj<T>& refObject)
	{
		mObject = refObject.mObject;
		if (mObject)
			mObject->incRefCount();
		return *this;
	}
};

CLOSE_O2_NAMESPACE

#endif // REF_COUNTER_H
