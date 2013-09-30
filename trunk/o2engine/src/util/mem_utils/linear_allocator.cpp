#include "linear_allocator.h"

OPEN_O2_NAMESPACE

cLinearAllocator::cLinearAllocator( unsigned int size, IAllocator* parentAllocator /*= NULL*/ ):
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
}

void* cLinearAllocator::alloc( uint32 bytes )
{
	if (mUsedMemory + bytes >= mMemorySize)
		return NULL;
	else
	{
		void* res = mMemory + mUsedMemory;
		mUsedMemory +=  bytes;
		return res;
	}
}

void* cLinearAllocator::realloc( void* ptr, uint32 bytes )
{
	void* res = alloc(bytes);
	memcpy(res, ptr, bytes);
	return res;
}

void cLinearAllocator::free( void* ptr )
{

}

CLOSE_O2_NAMESPACE