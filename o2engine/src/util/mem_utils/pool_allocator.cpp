#include "pool_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

	//void mfree(void* ptr) { free(ptr); }

PoolAllocator::PoolAllocator( uint32 chunksCount, uint16 chunkSize /*= 4*/, IAllocator* parentAllocator /*= NULL*/ ):
	mParentAllocator(parentAllocator)
{
	mMemorySize = chunksCount*chunkSize;
	mChunkSize = chunkSize;
	mChunksCount = chunksCount;

	uint32 mutexSize = sizeof(Mutex);

	if (parentAllocator)
	{
		mMemory = (char*)ALLOC(parentAllocator, mMemorySize + mutexSize);
	}
	else
	{
		mMemory = (char*)malloc(mMemorySize + mutexSize);
	}

	for (uint32 i = 0; i < mChunksCount - 1; i++)
	{
		*(uint32*)(mMemory + i*mChunkSize) = (uint32)(mMemory + (i + 1)*mChunkSize);
	}
	*(uint32*)(mMemory + (mChunksCount - 1)*mChunkSize) = NULL;

	mHead = mMemory;

	mMutex = new (mMemory + mMemorySize) Mutex;
}

PoolAllocator::~PoolAllocator()
{
	mMutex->~Mutex();

	if (mParentAllocator)
	{
		FREE(mParentAllocator, mMemory);
	}
	else
	{
		mfree(mMemory);
	}
}

void* PoolAllocator::alloc( uint32 bytes )
{	
	mMutex->lock();

	if (!mHead)
		return NULL;

	void* res = mHead;
	mHead = (char*)(*(uint32*)mHead);

	mMutex->unlock();

	return res;
}

void* PoolAllocator::realloc( void* ptr, uint32 bytes )
{
	return ptr;
}

void PoolAllocator::free( void* ptr )
{
	mMutex->lock();

	if (!ptr)
		return;
	
	*(uint32*)(ptr) = (uint32)(mHead);
	mHead = (char*)ptr;

	mMutex->unlock();
}

CLOSE_O2_NAMESPACE