#ifndef UI_STATE_H
#define UI_STATE_H

#include <map>

#include "public.h"
#include "util/property.h"
#include "util/callback.h"

OPEN_O2_NAMESPACE

#define DEBUG_REFS

struct RefObjBase
{
	struct place 
	{
		void* mRef;
		char  mPlace[256];
	};

	typedef vector<place> PlacesVec;

	char      mInitPlace[256];
	PlacesVec mRefPlaces;

	void push(void* ref, );
	void pull(const string& place);
};

template<typename T>
class RefObj
{
protected:
	T*            mObject;
	unsigned int* mRefCount;

public:
	RefObj(T* object) 
	{
		initialize(object);
	}

	RefObj(const RefObj<T>& ref) 
	{
		mObject = ref.mObject;
		mRefCount = ref.mRefCount;
		*mRefCount += 1;
	}

	~RefObj() 
	{
		release();
	}

	T* operator->() { return mObject; }
	T& operator*() { return *mObject; }

	RefObj& operator=(T* object) 
	{
		release();
		initialize(object);
	}

	RefObj& operator=(RefObj* ref) 
	{
		release();
		initialize(ref);
	}

protected:
	void initialize(T* object) 
	{
		mObject = object;
		mRefCount = mnew unsigned int;
		*mRefCount = 1;
	}

	void initialize(const RefObj<T>& ref) 
	{
		mObject = ref.mObject;
		mRefCount = ref.mRefCount;
		*mRefCount += 1;
	}

	void release()
	{
		*mRefCount -= 1;
		if (*mRefCount == 0) 
		{
			safe_release(mObject);
			safe_release(mRefCount);
		}
	}
};



#ifdef DEBUG_REFS
#	define ref(object)
#else
#endif

class uiState
{
	string     mName;
	cCallbackChain* mOnActiveStateCallbacks;
	cCallbackChain* mOnDeactiveStateCallbacks;

public: 
	PROPERTY(uiState, int) state;

	uiState(const string& name, int maxStatesCount = 2);
	virtual ~uiState();

	virtual void setState(const int& state) {}
	virtual int getState() const {}

	virtual void update(float dt) {}

	ICallback* registOnStateActivatedCallback(ICallback* callback, int stateId);
	void unregistOnStateActivatedCallback(ICallback* callback);
	void unregistAllOnActivatedStateCallbacks();

	ICallback* registOnStateDeactivatedCallback(ICallback* callback, int stateId);
	void unregistOnStateDeactivatedCallback(ICallback* callback);
	void unregistAllOnDeactivatedStateCallbacks();

protected:
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_STATE_H
