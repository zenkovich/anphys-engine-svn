#ifndef SMART_PTRS_H
#define SMART_PTRS_H

#include "public.h"

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

CLOSE_O2_NAMESPACE

#endif // SMART_PTRS_H
