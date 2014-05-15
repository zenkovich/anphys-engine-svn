#ifndef SMART_PTRS_H
#define SMART_PTRS_H

#include "util/public_namespace.h"

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

void xlog(const char* res);

/** Object that defines a shared type. Object containing reference count and some checking functionality, as default 
 ** releasing object, when reference count is zero. */
template<typename T>
class _shared
{
public:
	T*            mObject;
	unsigned int* mRefCount;
	bool*         mValid;
	bool*         mAutoRelease;

	_shared()
	{
		mObject = NULL;
		mRefCount = NULL;
		mValid = NULL;
		mAutoRelease = NULL;
	}

	_shared(T* object) 
	{
		initialize(object);
	}

	_shared(const _shared<T>& ref) 
	{
		initialize(ref);
	}

	template<typename P>
	_shared(const _shared<P>& ref) 
	{
		initialize(ref);
	}

	~_shared() 
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

	_shared& operator=(T* object) 
	{
		release();
		initialize(object);
		return *this;
	}

	bool operator==(const _shared<T>& ref)
	{
		return ref.mObject == mObject;
	}

	bool operator==(const _shared<T>& ref) const
	{
		return ref.mObject == mObject;
	}

	bool operator!=(const _shared<T>& ref)
	{
		return ref.mObject != mObject;
	}

	bool operator!=(const _shared<T>& ref) const
	{
		return ref.mObject != mObject;
	}

	_shared<T>& operator=(const _shared<T>& ref) 
	{
		release();
		initialize(ref);
		return *this;
	}

	template<typename P>
	_shared<T>& operator=(const _shared<P>& ref) 
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

	operator bool() const
	{
		checkValid();
		return mObject != NULL;
	}

	_shared<T>& setAutoRelease(bool enable)
	{
		if (!mRefCount)
			return *this;

		checkValid();
		*mAutoRelease = enable;

		return *this;
	}
	
	_shared<T>& enableAutoRelease()
	{
		return setAutoRelease(true);
	}
	
	_shared<T>& disableAutoRelease()
	{
		return setAutoRelease(false);
	}

	void* force_release()
	{
		void* res = NULL;
		if (mObject)
		{
			if (!*mValid)
			{
				xlog("Failed to delete object: object already deleted!");
				*mRefCount -= 1;
			}
			else
			{
				res = mObject;

				*mValid = false;

				if (*mRefCount > 1)
					xlog("Possible using destroyed object - there are %i links on this object");

				*mRefCount = 1;

				release();

				initialize(NULL);
			}
		}
		else
		{
			xlog("Failed to delete object - object is null!");
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
			mAutoRelease = NULL;
			return;
		}

		mObject = object;
		mRefCount = new unsigned int;
		*mRefCount = 1;
		mValid = new bool;
		*mValid = true;
		mAutoRelease = new bool;
		*mAutoRelease = true;
	}

	template<typename P>
	void initialize(const _shared<P>& ref) 
	{
		if (ref.mValid && *(ref.mValid) == false)
			xlog("Using not valid pointer - at pointer initialization");

		mObject = (T*)ref.mObject;
		mRefCount = ref.mRefCount;
		mValid = ref.mValid;
		mAutoRelease = ref.mAutoRelease;

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
			{
				delete mRefCount;
				delete mValid;

				if (*mAutoRelease)
					delete mObject;

				delete mAutoRelease;
			}
		}
	}

	void checkValid() const
	{
		if (mRefCount == 0)
			return;

		if (*mValid == false)
			xlog("Using not valid pointer");
	}
};

/** Object that defines a shared array type. Object containing reference count and some checking functionality, as default 
 ** releasing object, when reference count is zero. */
template<typename T>
class _shared_arr
{
public:
	T*            mObject;
	unsigned int* mRefCount;
	bool*         mValid;
	bool*         mAutoRelease;

	_shared_arr()
	{
		mObject = NULL;
		mRefCount = NULL;
		mValid = NULL;
		mAutoRelease = NULL;
	}

	_shared_arr(T* object) 
	{
		initialize(object);
	}

	_shared_arr(const _shared_arr<T>& ref) 
	{
		initialize(ref);
	}

	template<typename P>
	_shared_arr(const _shared_arr<P>& ref) 
	{
		initialize(ref);
	}

	~_shared_arr() 
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

	_shared_arr& operator=(T* object) 
	{
		release();
		initialize(object);
		return *this;
	}

	bool operator==(const _shared_arr& ref)
	{
		return ref.mObject == mObject;
	}

	_shared_arr<T>& operator=(const _shared_arr<T>& ref) 
	{
		release();
		initialize(ref);
		return *this;
	}

	bool operator!=(const _shared_arr<T>& ref)
	{
		return ref.mObject != mObject;
	}

	bool operator!=(const _shared_arr<T>& ref) const
	{
		return ref.mObject != mObject;
	}

	template<typename P>
	_shared_arr<T>& operator=(const _shared_arr<P>& ref) 
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

	_shared_arr<T>& setAutoRelease(bool enable)
	{
		if (!mRefCount)
			return *this;

		checkValid();
		*mAutoRelease = enable;

		return *this;
	}
	
	_shared_arr<T>& enableAutoRelease()
	{
		return setAutoRelease(true);
	}
	
	_shared_arr<T>& disableAutoRelease()
	{
		return setAutoRelease(false);
	}

	void* force_release()
	{
		void* res = NULL;
		if (mObject)
		{
			if (!*mValid)
			{
				xlog("Failed to delete object: object already deleted!");
				*mRefCount -= 1;
			}
			else
			{
				res = mObject;

				*mValid = false;

				if (*mRefCount > 0)
					xlog("Possible using destroyed object - there are %i links on this object");

				*mRefCount = 1;

				release();

				initialize(NULL);
			}
		}
		else
		{
			xlog("Failed to delete object - object is null!");
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
			mAutoRelease = NULL;
			return;
		}

		mObject = object;
		mRefCount = new unsigned int;
		*mRefCount = 1;
		mValid = new bool;
		*mValid = true;
		mAutoRelease = new bool;
		*mAutoRelease = true;
	}

	template<typename P>
	void initialize(const _shared_arr<P>& ref) 
	{
		if (ref.mValid && *(ref.mValid) == false)
			xlog("Using not valid pointer - at pointer initialization");

		mObject = (T*)ref.mObject;
		mRefCount = ref.mRefCount;
		mValid = ref.mValid;
		mAutoRelease = ref.mAutoRelease;

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
			{
				delete mRefCount;
				delete mValid;

				if (*mAutoRelease)
					delete[] mObject;

				delete mAutoRelease;
			}
		}
	}

	void checkValid() const
	{
		if (mRefCount == 0)
			return;

		if (*mValid == false)
			xlog("Using not valid pointer");
	}
};



#define shared(type) _shared<type>
#define sharedArr(type) _shared_arr<type>

template<typename T>
void* _safe_release(_shared<T>& ptr)
{
	return ptr.force_release();
}

template<typename T>
void* _safe_release_arr(_shared_arr<T>& ptr)
{
	return ptr.force_release();
}

CLOSE_O2_NAMESPACE

#endif // SMART_PTRS_H
