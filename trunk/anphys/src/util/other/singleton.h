#ifndef SINGLETON_H
#define SINGLETON_H

#include "../debug/assert.h"

template <typename T> class cSingleton
{
public:
	cSingleton() { mInstance = static_cast<T*>(this); }
	virtual ~cSingleton() { mInstance = NULL; }

	static T& instance() { assert(mInstance, "Singleton not initialized"); return *mInstance; }
	
	static T* instancePtr() { return mInstance; }

	static void initializeSingleton() { if (!mInstance) mInstance = new T; }
	static void deinitializeSingleton() { SAFE_RELEASE(mInstance); }

	static bool isSingletonInitialzed() { return (mInstance != NULL); }

protected:
	static T* mInstance;
};


#define DECLARE_SINGLETONE(T) template<> T* cSingleton<T>::mInstance = NULL;

#endif // SINGLETON_H