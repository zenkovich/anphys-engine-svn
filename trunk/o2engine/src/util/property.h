#ifndef PROPERTY_H
#define PROPERTY_H

#include "public.h"

#include "callback.h"

OPEN_O2_NAMESPACE

template<typename T>
class cProperty
{
	T          mObject;
	T          mLastObject;
	ICallback* mChangeCallback;

public:
	cProperty() :mObject(), mLastObject(), mChangeCallback(0) {}

	cProperty(const T& object) :mObject(object), mLastObject(object), mChangeCallback(NULL) {}

	~cProperty()
	{
		safe_release(mChangeCallback);
	}


	void setChangeCallback(ICallback* callback)
	{
		safe_release(mChangeCallback);
		mChangeCallback = callback;
	}

	bool check()
	{
		if (mObject != mLastObject)
		{
			mLastObject = mObject;
			return true;
		}

		return false;
	}

	cProperty& operator=(const cProperty& prop)
	{
		*this = prop.mObject;
		return *this;
	}

	cProperty& operator=(const T& obj)
	{
		if (mObject != obj)
		{
			mObject = obj;
			if (mChangeCallback)
				mChangeCallback->call();
		}

		return *this;
	}

	operator T&() 
	{
		return mObject;
	}

	T* operator ->()
	{
		return &mObject;
	}

	T& operator *()
	{
		return mObject;
	}

	const T& operator *() const
	{
		return mObject;
	}
};

CLOSE_O2_NAMESPACE

#endif //PROPERTY_H