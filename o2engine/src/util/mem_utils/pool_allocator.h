#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include "allocator_interface.h"
#include <Windows.h>

OPEN_O2_NAMESPACE
	
class Mutex;

class PoolAllocator:public IAllocator
{
	IAllocator* mParentAllocator;

	struct chunk
	{
		char* mData;
	};

	char*   mMemory;
	char*   mHead;
	uint32  mMemorySize;
	uint16  mChunkSize;
	uint32  mChunksCount;
	
	Mutex* mMutex;

public:
	PoolAllocator(uint32 chunksCount, uint16 chunkSize = 16, IAllocator* parentAllocator = NULL);
	~PoolAllocator();

	void* alloc(uint32 bytes);
	void* realloc(void* ptr, uint32 bytes);
	void free(void* ptr);

	const char* getName() const { return "pool alloc"; }
};

CLOSE_O2_NAMESPACE

#endif //POOL_ALLOCATOR_H