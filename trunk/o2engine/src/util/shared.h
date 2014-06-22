#ifndef SHARED_H
#define SHARED_H

#include "public_namespace.h"
#include "public_types.h"
#include "stack_trace.h"
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
	enum { nCheckSectionSize = 8, checkMagicNumber = 88 };

public:
	char        mCheckSection[nCheckSectionSize];
	bool        mAutoRelease;
	cSharedBase mSharedBase;

	void* operator new(uint32 size) 
	{
		void* res = malloc(size);
		memset(res, checkMagicNumber, nCheckSectionSize);
		return res;
	}

	void* operator new(uint32 size, const char* location, int line) 
	{
		void* res = malloc(size);
		memset(res, checkMagicNumber, nCheckSectionSize);
		return res;
	}

	void operator delete(void* ptr)
	{
		free(ptr);
	}

	void operator delete(void* ptr, const char* location, int line)
	{
		free(ptr);
	}

	void yesIAmSharedObject() {}

	cShareObject()
	{
		bool isAutoRelease = true;
		char* checkSection = (char*)this;
		for (int i = 0; i < nCheckSectionSize; i++)
		{
			if (checkSection[i] != checkMagicNumber)
			{
				isAutoRelease = false;
				break;
			}
		}

		mAutoRelease = isAutoRelease;
	}
};

struct IShared
{
	bool         mValid;
	std::string  mStackTrace;
	cSharedBase* mBase;
	IShared*     mParent;

	IShared():mValid(true), mBase(NULL), mParent(NULL) {}

	void checkTraceStack() { mStackTrace = getStackTraceAsStr(); }
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

	shared<_type> setParent(void* parent) 
	{
		mParent = dynamic_cast<IShared>(parent);
		/*IShared* par = parent;
		while(par)
		{
			if (par == )
		}*/
		return *this;
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
		checkTraceStack();
		mBase = &mObject->mSharedBase;
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
		checkTraceStack();
		mBase = &mObject->mSharedBase;
	}

	void release()
	{
		if (!mObject)
			return;			
		
		mObject->mSharedBase.eraseRef(this);

		if (!mObject->mSharedBase.isHereAnybody())
		{
			//SharedsRelease((void*)mObject);
			if (mObject->mAutoRelease)
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

#define sharex(object) (shared(mnew object)).setParent(this)
#define share(object, parent) (object).setParent(parent)

template<typename T>
T* _safe_release(shared<T>& ptr)
{
	return ptr.force_release();
}

CLOSE_O2_NAMESPACE

#endif // SHARED_H
