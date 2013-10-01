#include "linear_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

cLinearAllocator::cLinearAllocator( uint32 size, IAllocator* parentAllocator /*= NULL*/ ):
	mParentAllocator(parentAllocator), mMemorySize(size)
{
	if (parentAllocator)
	{
		mMemory = (char*)ALLOC(parentAllocator, size);
	}
	else
	{
		mMemory = (char*)malloc(size);
	}

	mMutex = mnew cMutex;

	mUsedMemory = 0;
}

cLinearAllocator::~cLinearAllocator()
{
	if (mParentAllocator)
	{
		FREE(mParentAllocator, mMemory);
	}
	else
	{
		free(mMemory);
	}

	safe_release(mMutex);
}

void* cLinearAllocator::alloc( uint32 bytes )
{
	mMutex->lock();

	if (mUsedMemory + bytes >= mMemorySize)
	{
		return NULL;
		mMutex->unlock();
	}
	else
	{
		void* res = mMemory + mUsedMemory;
		mUsedMemory +=  bytes;
		return res;

		mMutex->unlock();
	}

	return NULL;
}

void* cLinearAllocator::realloc( void* ptr, uint32 bytes )
{
	mMutex->lock();

	void* res = alloc(bytes);
	memcpy(res, ptr, min<uint32>(bytes, mMemory + mUsedMemory - (char*)ptr));

	mMutex->unlock();

	return res;
}

void cLinearAllocator::free( void* ptr )
{
}

CLOSE_O2_NAMESPACE