#ifndef SMART_PTRS_H
#define SMART_PTRS_H

#include "public.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

template<typename T>
class cAutoPtr
{
	T* mObjPtr;

public:
	cAutoPtr():mObjPtr(NULL) {}
	cAutoPtr(T* objPtr) { mObjPtr = objPtr; }
	~cAutoPtr() { safe_release(mObjPtr); }

	bool isInitialized() const { return mObjPtr != NULL; }
	T* get() { return mObjPtr; }

	operator T*() { return mObjPtr; }
	T* operator->() { return mObjPtr; }
	T& operator*() { return *mObjPtr; }
	
	cAutoPtr& operator=(T* objPtr)
	{
		safe_release(mObjPtr);
		mObjPtr = objPtr;
		return *this;
	}

private:
	cAutoPtr& operator=(const cAutoPtr* ptr) { return *this; }
};

template<typename T>
class SafePtr
{
	typedef T* tp;

protected:
	T*            mObject;
	unsigned int* mRefCount;
	bool          mIsBasic;

public:
	SafePtr()
	{
		mObject = NULL;
		mRefCount = NULL;
	}

	SafePtr(T* object) 
	{
		initialize(object);
	}

	SafePtr(const SafePtr<T>& ref) 
	{
		initialize(ref);
	}

	~SafePtr() 
	{
		release();
	}

	operator tp() 
	{
		return mObject;
	}

	T* operator->() 
	{
		if (mRefCount == NULL) 
			logError("Using unitialized pointer %x!", this);

		return mObject;
	}

	T* const operator->() const
	{
		if (mRefCount == NULL) 
			logError("Using unitialized pointer %x!", this);

		return mObject;
	}

	T& operator*() 
	{ 
		if (mRefCount == NULL)
			logError("Using unitialized pointer %x!", this);

		return *mObject;
	}

	SafePtr& operator=(T* object) 
	{
		release();
		initialize(object);
		return *this;
	}

	bool operator==(const SafePtr& ref)
	{
		return ref.mObject == mObject;
	}

	SafePtr& operator=(const SafePtr& ref) 
	{
		release();
		initialize(ref);
		return *this;
	}

	operator bool()
	{
		return mObject != NULL;
	}

protected:
	void initialize(T* object) 
	{
		if (object == NULL)
		{
			mObject = NULL;
			mRefCount = NULL;
			mIsBasic = false;
			return;
		}

		mObject = object;
		mRefCount = mnew unsigned int;
		*mRefCount = 1;
		mIsBasic = true;
	}

	void initialize(const SafePtr<T>& ref) 
	{
		mObject = ref.mObject;
		mRefCount = ref.mRefCount;

		if (mRefCount)
			*mRefCount += 1;

		mIsBasic = false;
	}

	void release()
	{
		if (!mRefCount)
			return;

		*mRefCount -= 1;
		if (*mRefCount == 0)
		{
			safe_release(mRefCount);
		}
		else if (mIsBasic) 
		{
			logError("At destroying base object by %x: there are %i unreleased links!", this, *mRefCount);
		}
	}
};


#ifdef DEBUG_POINTERS
#	define ptr(type) SafePtr<type>
#else
#	define ptr(type) type*
#endif

CLOSE_O2_NAMESPACE

#endif // SMART_PTRS_H
