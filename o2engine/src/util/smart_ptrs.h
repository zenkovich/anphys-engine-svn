#ifndef SMART_PTRS_H
#define SMART_PTRS_H

#include "public.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

template<typename T, bool arr>
class cAutoPtr
{
	T* mObjPtr;

public:
	cAutoPtr():mObjPtr(NULL) {}

	cAutoPtr(T* objPtr) 
	{
		mObjPtr = objPtr; 
	}

	~cAutoPtr() 
	{
		if(arr)
		{
			safe_release_arr(mObjPtr);
		}
		else 
		{
			safe_release(mObjPtr);
		}
	}

	bool isInitialized() const
	{
		return mObjPtr != NULL;
	}

	T* get() 
	{
		return mObjPtr;
	}

	operator bool() { return mObjPtr != NULL; }
	operator T*() { return mObjPtr; }
	T* operator->() { return mObjPtr; }
	T& operator*() { return *mObjPtr; }
	
	cAutoPtr& operator=(T* objPtr)
	{
		if(arr)
		{
			safe_release_arr(mObjPtr); 
		}
		else 
		{
			safe_release(mObjPtr);
		}
		mObjPtr = objPtr;
		return *this;
	}

private:
	cAutoPtr& operator=(const cAutoPtr* ptr) { return *this; }
};

#define autoPtr(type) cAutoPtr<type, false>
#define autoArr(type) cAutoPtr<type, true>


template<typename T>
class SafePtr
{
protected:
	T*            mObject;
	unsigned int* mRefCount;
	bool*         mValid;

public:
	SafePtr()
	{
		mObject = NULL;
		mRefCount = NULL;
		mValid = NULL;
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

	template<typename P>
	operator P*()
	{
		checkValid();
		return (P*)mObject;
	}

	operator T*() 
	{
		checkValid();
		return mObject;
	}

	T* operator->() 
	{
		checkValid();
		return mObject;
	}

	T* const operator->() const
	{
		checkValid();
		return mObject;
	}

	T& operator*() 
	{ 
		checkValid();
		return *mObject;
	}

	T& operator*() const
	{ 
		checkValid();
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
		checkValid();
		return mObject != NULL;
	}

	void* force_release()
	{
		void* res = NULL;
		if (mObject)
		{
			if (!*mValid)
			{
				logError("Failed to delete object %x: object already deleted!");
				*mRefCount -= 1;
			}
			else
			{
				res = mObject;

				*mValid = false;

				release();

				if (*mRefCount > 0)
					logWarning("Possible using destroyed object %x - there are %i links on this object", res, *mRefCount);

				initialize(NULL);
			}
		}
		else
		{
			logError("Failed to delete object - object is null!");
		}

		return res;
	}

protected:
	void initialize(T* object) 
	{
		if (object == NULL)
		{
			mObject = NULL;
			mRefCount = NULL;
			mValid = NULL;
			return;
		}

		mObject = object;
		mRefCount = mnew unsigned int;
		*mRefCount = 1;
		mValid = mnew bool;
		*mValid = true;
	}

	void initialize(const SafePtr<T>& ref) 
	{
		if (ref.mValid && *(ref.mValid) == false)
			logError("Using not valid pointer %x - at pointer initialization", ref.mObject);

		mObject = ref.mObject;
		mRefCount = ref.mRefCount;
		mValid = ref.mValid;

		if (mRefCount)
			*mRefCount += 1;
	}

	void release()
	{
		if (!mRefCount)
			return;

		*mRefCount -= 1;
		if (*mRefCount == 0)
		{
			if (*mValid)
				logError("Memory leak: object %x was not destroyed, but all pointers are released!");

			safe_release(mRefCount);
			safe_release(mValid);
		}
	}

	void checkValid() const
	{
		if (mRefCount == NULL)
			return;

		if (*mValid == false)
			logError("Using not valid pointer %x", mObject);
	}
};


#ifdef DEBUG_POINTERS
#	define ptr(type) SafePtr<type>

	template<typename T>
	void* safe_release_(SafePtr<T>& ptr)
	{
		return ptr.force_release();
	}

	template<typename T>
	void* safe_release_arr_(SafePtr<T>& ptr)
	{
		return ptr.force_release();
	}

#else
#	define ptr(type) type*
#endif

CLOSE_O2_NAMESPACE

#endif // SMART_PTRS_H
