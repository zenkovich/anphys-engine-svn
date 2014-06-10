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
	bool mValid;
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

/*	bool operator==(const shared<_type>& ref)
	{
		return ref.mObject == mObject;
	}

	bool operator==(const shared<_type>& ref) const
	{
		return ref.mObject == mObject;
	}

	bool operator!=(const shared<_type>& ref)
	{
		return ref.mObject != mObject;
	}

	bool operator!=(const shared<_type>& ref) const
	{
		return ref.mObject != mObject;
	}	
	
	template<typename P>
	bool operator==(const P* ptr)
	{
		return ptr == mObject;
	}
	
	template<typename P>
	bool operator==(const P* ptr) const
	{
		return ptr == mObject;
	}
	*/
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

	void* force_release()
	{
		void* res = NULL;
		if (mObject)
		{
			if (!mValid)
				SharedsErrorOut("Trying to release not valid object reference");
			
			cShareObject* shareObj = (cShareObject*)mObject;
			shareObj->mSharedBase.eraseRef(this);

			for(vector<IShared*>::iterator ref = shareObj->mSharedBase.mReferences.begin(); ref != shareObj->mSharedBase.mReferences.end(); ++ref)
				(*ref)->mValid = false;

			if (shareObj->mSharedBase.isHereAnybody())
				SharedsErrorOut("Hey, you forgot something! There some nonvalid objects", false);

			res = (void*)mObject;
			mObject = NULL;
		}
		else
		{
			SharedsErrorOut("Failed to delete object - object is NULL");
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

		cShareObject* shareObj = (cShareObject*)mObject;
		shareObj->mSharedBase.pushRef(this);
	}

	template<typename P>
	void initialize(const shared<P>& ref) 
	{
		if (ref.mObject && !ref.mValid)
			SharedsErrorOut("Failed to initialize shared: not valid object");

		mObject = static_cast<_type*>(ref.mObject);

		if (!mObject)
			return;
		
		cShareObject* shareObj = (cShareObject*)mObject;
		shareObj->mSharedBase.pushRef(this);
	}

	void release()
	{
		if (!mObject)
			return;		
		
		cShareObject* shareObj = (cShareObject*)mObject;
		shareObj->mSharedBase.eraseRef(this);

		if (!shareObj->mSharedBase.isHereAnybody())
			SharedsRelease((void*)mObject);
	}

	void checkValid() const
	{
		if (mObject && !mValid)
			SharedsErrorOut("Wow, maan, you are using dead pointer..");
	}
};

template<typename T>
void* _safe_release(shared<T>& ptr)
{
	return ptr.force_release();
}

CLOSE_O2_NAMESPACE

#endif // SHARED_H
