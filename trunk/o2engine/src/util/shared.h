#ifndef SHARED_H
#define SHARED_H

#include "public_namespace.h"
#include "public_types.h"
#include <vector>
#include <algorithm>

OPEN_O2_NAMESPACE
	
struct IShared;

void SharedsErrorOut(const char* res, bool asserting = true);
void SharedsRelease(void* ptr);

struct cSharedBase
{
	vector<IShared*> mReferences;

	void pushRef(IShared* ref) 
	{
		mReferences.push_back(ref); 
	}

	void eraseRef(IShared* ref)
	{
		mReferences.erase(std::find(mReferences.begin(), mReferences.end(), ref));
	}

	bool isHereAnybody() 
	{
		return mReferences.size() > 0;
	}
};

class cShareObject
{
public:
	cSharedBase mSharedBase;

	void yesIAmSharedObject() {}
};

struct IShared
{
	bool         mValid;

	IShared():mValid(true) {}
};

template<typename _type>
class shared: public IShared
{
public:
	_type* mObject;

	shared()
	{
		mObject = NULL;
	}

	shared(_type* object) 
	{
		initialize(object);
	}

	shared(const shared<_type>& ref) 
	{
		initialize(ref);
	}

	~shared() 
	{
		release();
	}

	template<typename _conv_type>
	operator shared<_conv_type>()
	{
		checkValid();

		shared<_conv_type> res;
		res.initialize((_conv_type*)(mObject));

		return res;
	}

	template<typename _conv_type>
	operator _conv_type*()
	{
		checkValid();
		return static_cast<_conv_type*>(mObject);
	}

	operator _type*() 
	{
		checkValid();
		return mObject;
	}

	_type* operator->() 
	{
		checkValid();
		return mObject;
	}

	_type* const operator->() const
	{
		checkValid();
		return mObject;
	}

	_type& operator*() 
	{ 
		checkValid();
		return *mObject;
	}

	_type& operator*() const
	{ 
		checkValid();
		return *mObject;
	}

	shared& operator=(_type* object) 
	{
		release();
		initialize(object);
		return *this;
	}

	template<typename P>
	bool operator!=(P* const ptr)
	{
		return ptr != mObject;
	}
	
	template<typename P>
	bool operator!=(P* const ptr) const
	{
		return ptr != mObject;
	}

	shared<_type>& operator=(const shared<_type>& ref) 
	{
		release();
		initialize(ref);
		return *this;
	}

	template<typename P>
	shared<_type>& operator=(const shared<P>& ref) 
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

	_type* force_release()
	{
		_type* res = NULL;
		if (mObject)
		{
			if (!mValid)
				SharedsErrorOut("Trying to release not valid object reference");
			
			mObject->mSharedBase.eraseRef(this);

			for(vector<IShared*>::iterator ref = mObject->mSharedBase.mReferences.begin(); ref != mObject->mSharedBase.mReferences.end(); ++ref)
				(*ref)->mValid = false;

			if (mObject->mSharedBase.isHereAnybody())
				SharedsErrorOut("Hey, you forgot something! There some nonvalid objects", false);

			res = mObject;
			mObject = NULL;
		}

		return res;
	}

public:
	void initialize(_type* object) 
	{
		object->yesIAmSharedObject();
		mObject = object;

		if (!mObject)
			return;

		mObject->mSharedBase.pushRef(this);
	}

	template<typename P>
	void initialize(const shared<P>& ref) 
	{
		if (ref.mObject && !ref.mValid)
			SharedsErrorOut("Failed to initialize shared: not valid object");

		mObject = static_cast<_type*>(ref.mObject);

		if (!mObject)
			return;		
		
		mObject->mSharedBase.pushRef(this);
	}

	void release()
	{
		if (!mObject)
			return;			
		
		mObject->mSharedBase.eraseRef(this);

		if (!mObject->mSharedBase.isHereAnybody())
		{
			//SharedsRelease((void*)mObject);
			delete mObject;
			mValid = false;
		}
	}

	void checkValid() const
	{
		if (mObject && !mValid)
			SharedsErrorOut("Wow, maan, you are using dead pointer..");
	}
};

template<typename _type>
class weak: public IShared
{
public:
	_type* mObject;

	weak()
	{
		mObject = NULL;
	}

	weak(_type* object) 
	{
		initializeWeak(object);
	}

	weak(const weak<_type>& ref) 
	{
		initializeWeak(ref);
	}

	weak(const shared<_type>& ref) 
	{
		initializeWeak(ref);
	}

	~weak() 
	{
		release();
	}

	template<typename _conv_type>
	operator weak<_conv_type>()
	{
		checkValid();

		weak<_conv_type> res;
		res.initializeWeak((_conv_type*)(mObject));

		return res;
	}

	template<typename _conv_type>
	operator shared<_conv_type>()
	{
		checkValid();

		shared<_conv_type> res;
		res.initialize((_conv_type*)(mObject));

		return res;
	}

	template<typename _conv_type>
	operator _conv_type*()
	{
		checkValid();
		return static_cast<_conv_type*>(mObject);
	}

	operator _type*() 
	{
		checkValid();
		return mObject;
	}

	_type* operator->() 
	{
		checkValid();
		return mObject;
	}

	_type* const operator->() const
	{
		checkValid();
		return mObject;
	}

	_type& operator*() 
	{ 
		checkValid();
		return *mObject;
	}

	_type& operator*() const
	{ 
		checkValid();
		return *mObject;
	}

	weak& operator=(_type* object) 
	{
		release();
		initializeWeak(object);
		return *this;
	}

	template<typename P>
	bool operator!=(P* const ptr)
	{
		return ptr != mObject;
	}
	
	template<typename P>
	bool operator!=(P* const ptr) const
	{
		return ptr != mObject;
	}

	weak<_type>& operator=(const weak<_type>& ref) 
	{
		release();
		initializeWeak(ref);
		return *this;
	}

	template<typename P>
	weak<_type>& operator=(const weak<P>& ref) 
	{
		release();
		initializeWeak(ref);
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

	_type* force_release()
	{
		_type* res = NULL;
		if (mObject)
		{
			if (!mValid)
				SharedsErrorOut("Trying to release not valid object reference");

			for(vector<IShared*>::iterator ref = mObject->mSharedBase.mReferences.begin(); ref != mObject->mSharedBase.mReferences.end(); ++ref)
				(*ref)->mValid = false;

			if (mObject->mSharedBase.isHereAnybody())
				SharedsErrorOut("Hey, you forgot something! There some nonvalid objects", false);

			res = mObject;
			mObject = NULL;
		}

		return res;
	}

public:
	void initializeWeak(_type* object) 
	{
		object->yesIAmSharedObject();
		mObject = object;
	}

	template<typename P>
	void initializeWeak(const weak<P>& ref) 
	{
		if (ref.mObject && !ref.mValid)
			SharedsErrorOut("Failed to initialize weak: not valid object");

		mObject = static_cast<_type*>(ref.mObject);
	}

	void release()
	{
	}

	void checkValid() const
	{
		if (mObject && !mValid)
			SharedsErrorOut("Wow, maan, you are using dead pointer..");
	}
};

template<typename T>
T* _safe_release(shared<T>& ptr)
{
	return ptr.force_release();
}

template<typename T>
T* _safe_release(weak<T>& ptr)
{
	return ptr.force_release();
}

CLOSE_O2_NAMESPACE

#endif // SHARED_H
