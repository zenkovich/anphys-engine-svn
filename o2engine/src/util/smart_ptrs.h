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
void xrelease(void* data);

class ShareObj
{
public:
	int  mRefCount;
	bool mValid;

	ShareObj()
	{
		mRefCount = 1;
		mValid = true;
	}

private:
	virtual void awShiieeetDontUseMe() {}
};

/** Object that defines a shared type. Object containing reference count and some checking functionality, as default 
 ** releasing object, when reference count is zero. */
template<typename T>
class shared
{
public:
	ShareObj* mShareObj;

protected:	
	T* value() 
	{ 
		T* res = dynamic_cast<T*>(mShareObj);
		if (res == 0)
			xlog("Failed to cast shared pointer object. Object not derived from ShareObj");
		return res;
	}

	T* value() const 
	{ 
		T* res = dynamic_cast<T*>(mShareObj);
		if (res == 0)
			xlog("Failed to cast shared pointer object. Object not derived from ShareObj");
		return res;
	}

public:

	shared()
	{
		initialize(NULL);
	}

	shared(T* object) 
	{
		initialize(object);
	}

	shared(const shared<T>& ref) 
	{
		initialize(ref);
	}

	template<typename P>
	shared(const shared<P>& ref) 
	{
		initialize(ref);
	}

	~shared() 
	{
		release();
	}

	template<typename P>
	operator P*()
	{
		checkValid();
		return (P*)value();
	}

	operator T*() 
	{
		checkValid();
		return value();
	}

	T* operator->() 
	{
		checkValid();
		return value();
	}

	T* const operator->() const
	{
		checkValid();
		return value();
	}

	T& operator*() 
	{ 
		checkValid();
		return *value();
	}

	T& operator*() const
	{ 
		checkValid();
		return *value();
	}

	shared& operator=(T* object) 
	{
		release();
		initialize(object);
		return *this;
	}

	bool operator==(const shared<T>& ref)
	{
		return ref.mShareObj == mShareObj;
	}

	bool operator==(const shared<T>& ref) const
	{
		return ref.mShareObj == mShareObj;
	}

	bool operator!=(const shared<T>& ref)
	{
		return ref.mShareObj != mShareObj;
	}

	bool operator!=(const shared<T>& ref) const
	{
		return ref.mShareObj != mShareObj;
	}

	shared<T>& operator=(const shared<T>& ref) 
	{
		release();
		initialize(ref);
		return *this;
	}

	template<typename P>
	shared<T>& operator=(const shared<P>& ref) 
	{
		release();
		initialize(ref);
		return *this;
	}

	operator bool()
	{
		checkValid();
		return mShareObj != NULL;
	}

	operator bool() const
	{
		checkValid();
		return mShareObj != NULL;
	}

	void* force_release()
	{
		void* res = NULL;
		if (mShareObj)
		{
			if (!mShareObj->mValid)
			{
				xlog("Failed to delete object: object already deleted!");
				mShareObj -= 1;
			}
			else
			{
				res = mShareObj;

				mShareObj->mValid = false;

				if (mShareObj->mRefCount > 1)
					xlog("Possible using destroyed object - there are %i links on this object");

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
			mShareObj = NULL;
			return;
		}

		mShareObj = dynamic_cast<ShareObj*>(object);
		if (mShareObj == 0)
			xlog("Failed to cast shared pointer object. Object not derived from ShareObj");
	}

	template<typename P>
	void initialize(const shared<P>& ref) 
	{
		if (ref.mShareObj != NULL && !ref.mShareObj->mValid)
			xlog("Using not valid pointer - at pointer initialization");

		mShareObj = ref.mShareObj;

		if (mShareObj)
			mShareObj->mRefCount++;
	}

	void release()
	{
		if (!mShareObj)
			return;

		mShareObj->mRefCount -= 1;
		if (mShareObj->mRefCount == 0)
		{
			if (mShareObj->mValid)
			{
				xrelease(mShareObj);
			}
		}
	}

	void checkValid() const
	{
		if (!mShareObj)
			return;

		if (!mShareObj->mValid)
			xlog("Using not valid pointer");
	}
};

template<typename T>
shared<T> getShared(ShareObj* sharedObj)
{
	shared<T> res;
	res.mShareObj = sharedObj;
	return res;
}

/** Object that defines a shared array type. Object containing reference count and some checking functionality, as default 
 ** releasing object, when reference count is zero. */
template<typename T>
class sharedArr
{
public:
	T*            mObject;
	unsigned int* mRefCount;
	bool*         mValid;
	bool*         mAutoRelease;

	sharedArr()
	{
		mObject = NULL;
		mRefCount = NULL;
		mValid = NULL;
		mAutoRelease = NULL;
	}

	sharedArr(T* object) 
	{
		initialize(object);
	}

	sharedArr(const sharedArr<T>& ref) 
	{
		initialize(ref);
	}

	template<typename P>
	sharedArr(const sharedArr<P>& ref) 
	{
		initialize(ref);
	}

	~sharedArr() 
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

	sharedArr& operator=(T* object) 
	{
		release();
		initialize(object);
		return *this;
	}

	bool operator==(const sharedArr& ref)
	{
		return ref.mObject == mObject;
	}

	sharedArr<T>& operator=(const sharedArr<T>& ref) 
	{
		release();
		initialize(ref);
		return *this;
	}

	bool operator!=(const sharedArr<T>& ref)
	{
		return ref.mObject != mObject;
	}

	bool operator!=(const sharedArr<T>& ref) const
	{
		return ref.mObject != mObject;
	}

	template<typename P>
	sharedArr<T>& operator=(const sharedArr<P>& ref) 
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

	sharedArr<T>& setAutoRelease(bool enable)
	{
		if (!mRefCount)
			return *this;

		checkValid();
		*mAutoRelease = enable;

		return *this;
	}
	
	sharedArr<T>& enableAutoRelease()
	{
		return setAutoRelease(true);
	}
	
	sharedArr<T>& disableAutoRelease()
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
	void initialize(const sharedArr<P>& ref) 
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



template<typename T>
void* _safe_release(shared<T>& ptr)
{
	return ptr.force_release();
}

template<typename T>
void* _safe_release_arr(sharedArr<T>& ptr)
{
	return ptr.force_release();
}

CLOSE_O2_NAMESPACE

#endif // SMART_PTRS_H
