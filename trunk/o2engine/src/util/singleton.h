#ifndef SINGLETON_H
#define SINGLETON_H

#include "public.h"
#include "smart_ptrs.h"
OPEN_O2_NAMESPACE

template <typename CLASS> class Singleton
{
public:
	Singleton()                        { mInstance = static_cast<CLASS*>(this); }
	virtual ~Singleton()               { mInstance = NULL; }

	static CLASS&   instance()          { o2assert(mInstance, "Singleton not initialized"); return *mInstance; }
			    
	static CLASS* instancePtr()         { return mInstance; }

	static void initializeSingleton()   { if (!mInstance) mnew CLASS; }
	static void deinitializeSingleton() { safe_release(mInstance); }

	static bool isSingletonInitialzed() { return (mInstance != NULL); }

protected:
	static CLASS* mInstance;
};

#define DECLARE_SINGLETON(CLASS) template<> CLASS* Singleton<CLASS>::mInstance = NULL
#define CREATE_SINGLETON(CLASS) template<> CLASS* Singleton<CLASS>::mInstance = mnew CLASS()

CLOSE_O2_NAMESPACE

#endif //SINGLETON_H