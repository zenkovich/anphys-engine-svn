#include "pool_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

cPoolAllocator::cPoolAllocator( uint32 chunksCount, uint16 chunkSize /*= 4*/, IAllocator* parentAllocator /*= NULL*/ ):
	mParentAllocator(parentAllocator)
{
	mMemorySize = chunksCount*chunkSize;
	mChunkSize = chunkSize;
	mChunksCount = chunksCount;

	uint32 offs = chunksCount*sizeof(uint32);

	if (parentAllocator)
	{
		mBasicMemory = (char*)ALLOC(parentAllocator, mMemorySize + offs);
	}
	else
	{
		mBasicMemory = (char*)malloc(mMemorySize + offs);
	}
	mMemory = mBasicMemory + offs;

	mChunkSizes = (uint32*)mBasicMemory;
	memset(mChunkSizes, 0, offs);

	dc = GetDC(NULL);

	//mMutex = mnew cMutex;
}

cPoolAllocator::~cPoolAllocator()
{
	if (mParentAllocator)
	{
		FREE(mParentAllocator, mBasicMemory);
	}
	else
	{
		//free(mBasicMemory);
	}

	//safe_release(mMutex);
}

void* cPoolAllocator::alloc( uint32 bytes )
{	
	//mMutex->lock();

	uint32 reqChunks = bytes/4 + ( bytes%4 > 0 ? 1:0 );

	void* res = NULL;

	uint32 curFreeChunksCount = 0;
	for (uint32 i = 0; i < mChunksCount; i++)
	{
		if (mChunkSizes[i] > 0)
		{
			curFreeChunksCount = 0;
			i += mChunkSizes[i] - 1;
		}
		else
		{
			curFreeChunksCount++;
			if (curFreeChunksCount == reqChunks)
			{
				res = mMemory + (i - reqChunks + 1)*mChunkSize;
				mChunkSizes[i - reqChunks + 1] = curFreeChunksCount;

				for (uint32 j = i - reqChunks + 2; j < i; j++)
					mChunkSizes[j] = 0;				

				for (uint32 j = i - reqChunks + 1; j < i; j++)
					SetPixel(dc, j%1024, j/1024 + 20, RGB(255, 0, 0));

				break;
			}
		}
	}

	//mMutex->unlock();

	return res;
}

void* cPoolAllocator::realloc( void* ptr, uint32 bytes )
{
	//mMutex->lock();
	
	//mMutex->unlock();

	return ptr;
}

void cPoolAllocator::free( void* ptr )
{
	//mMutex->lock();

	if (!ptr)
		return;

	int idx = (uint32)((char*)ptr - mMemory)/mChunkSize;

	for (uint32 i = idx; i < idx + mChunkSizes[idx]; i++)
		SetPixel(dc, i%1024, i/1024 + 20, RGB(255, 255, 255));

	mChunkSizes[idx] = 0;

	//mMutex->unlock();
}

CLOSE_O2_NAMESPACE