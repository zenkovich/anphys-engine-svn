#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include "allocator_interface.h"
#include <Windows.h>

OPEN_O2_NAMESPACE
	
class cMutex;

class cPoolAllocator:public IAllocator
{
	IAllocator* mParentAllocator;

	char*       mBasicMemory;
	char*       mMemory;
	uint32      mMemorySize;
	uint16      mChunkSize;
	uint32      mChunksCount;
	uint32*     mChunkSizes;

	HDC dc;

	cMutex*     mMutex;

public:
	cPoolAllocator(uint32 chunksCount, uint16 chunkSize = 16, IAllocator* parentAllocator = NULL);
	~cPoolAllocator();

	void* alloc(uint32 bytes);
	void* realloc(void* ptr, uint32 bytes);
	void free(void* ptr);

	const char* getName() const { return "pool alloc"; }
};

CLOSE_O2_NAMESPACE

#endif //POOL_ALLOCATOR_H