#include "std_allocator.h"

#include <stdlib.h>

OPEN_O2_NAMESPACE

StdAllocator::StdAllocator()
{

}

StdAllocator::~StdAllocator()
{

}

void* StdAllocator::alloc( uint32 bytes )
{
	return malloc(bytes);
}

void* StdAllocator::realloc( void* ptr, uint32 bytes )
{
	return mrealloc(ptr, bytes);
}

void StdAllocator::free( void* ptr )
{
	mfree(ptr);
}


CLOSE_O2_NAMESPACE