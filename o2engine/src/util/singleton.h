#ifndef SINGLETON_H
#define SINGLETON_H

#include "public.h"
#include "smart_ptrs.h"
OPEN_O2_NAMESPACE

template <typename CLASS> class cSingleton
{
public:
	cSingleton()                        { mInstance = static_cast<CLASS*>(this); }
	virtual ~cSingleton()               { mInstance = NULL; }

	static CLASS&   instance()          { assert(mInstance, "Singleton not initialized"); return *mInstance; }
			    
	static shared<CLASS> instancePtr()  { return mInstance; }

	static void initializeSingleton()   { if (!mInstance) mnew CLASS; }
	static void deinitializeSingleton() { safe_release(mInstance); }

	static bool isSingletonInitialzed() { return (mInstance != NULL); }

protected:
	static shared<CLASS> mInstance;
};

#define DECLARE_SINGLETON(CLASS) template<> shared<CLASS> cSingleton<CLASS>::mInstance = NULL
#define CREATE_SINGLETON(CLASS) template<> shared<CLASS> cSingleton<CLASS>::mInstance = mnew T

CLOSE_O2_NAMESPACE

#endif //SINGLETON_H