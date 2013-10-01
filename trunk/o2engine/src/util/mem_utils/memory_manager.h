#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <list>

#include "public.h"

OPEN_O2_NAMESPACE

class IAllocator;
class cMutex;

class cMemoryManager
{
	friend class IAllocator;

	struct AllocSign
	{
		void*        mPtr;
		uint32       mSize;
		char         mSource[64];
		unsigned int mSourceLine;
		IAllocator*  mAllocator;

		AllocSign(void* memPtr, uint32 size, const char* source, unsigned int sourceLine, IAllocator* allocator);
		AllocSign(const AllocSign& allocSign);		
		AllocSign& operator=(const AllocSign& allocSign);
	};
	typedef std::list<AllocSign> AllocSignsList;

	AllocSignsList* mAllocSigns;
	uint32          mUsedMemory;
	cMutex*         mAllocSignsMutex;

	static cMemoryManager mStaticObj;

public:
	IAllocator* mBasicAllocator;

	cMemoryManager();
	~cMemoryManager();

	static cMemoryManager& instance() { return mStaticObj; }

	static void dump();

	static void registAlloc(void* memPtr, uint32 size, const char* source, unsigned int sourceLine, IAllocator* allocator);
	static void unregistAlloc(void* memPtr);
};

CLOSE_O2_NAMESPACE

#endif //MEMORY_MANAGER_H