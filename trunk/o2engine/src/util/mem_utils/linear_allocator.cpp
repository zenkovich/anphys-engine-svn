#include "linear_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

LinearAllocator::LinearAllocator( uint32 size, IAllocator* parentAllocator /*= NULL*/ ):
	mParentAllocator(parentAllocator), mMemorySize(size)
{
	if (parentAllocator)
	{
		mMemory = (char*)ALLOC(parentAllocator, size + sizeof(Mutex));
	}
	else
	{
		mMemory = (char*)malloc(size + sizeof(Mutex));
	}

	mMutex = new (mMemory + mMemorySize) Mutex;

	mUsedMemory = 0;
}

LinearAllocator::~LinearAllocator()
{
	mMutex->~Mutex();

	if (mParentAllocator)
	{
		FREE(mParentAllocator, mMemory);
	}
	else
	{
		free(mMemory);
	}
}

void* LinearAllocator::alloc( uint32 bytes )
{
	mMutex->lock();

	void* res = NULL;

	if (mUsedMemory + bytes < mMemorySize)
	{
		void* res = mMemory + mUsedMemory;
		mUsedMemory +=  bytes;
	}

	mMutex->unlock();

	return NULL;
}

void LinearAllocator::free( void* ptr )
{
}

CLOSE_O2_NAMESPACE