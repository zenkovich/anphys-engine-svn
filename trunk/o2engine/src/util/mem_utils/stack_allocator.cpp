#include "stack_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

StackAllocator::StackAllocator( uint32 size, IAllocator* parentAllocator /*= NULL*/ ):
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

	mMutex = mnew Mutex;

	mUsedMemory = 0;
}

StackAllocator::~StackAllocator()
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

void* StackAllocator::alloc( uint32 bytes )
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
}

void* StackAllocator::realloc( void* ptr, uint32 bytes )
{
	mMutex->lock();

	void* res = alloc(bytes);
	memcpy(res, ptr, min<uint32>(bytes, mMemory + mUsedMemory - (char*)ptr));

	mMutex->unlock();

	return res;
}

void StackAllocator::free( void* ptr )
{
	mMutex->lock();

	mUsedMemory = (uint32)ptr - (uint32)mMemory;

	mMutex->unlock();
}

CLOSE_O2_NAMESPACE