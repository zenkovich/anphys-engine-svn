#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class Mutex;

class StackAllocator:public IAllocator
{
	IAllocator* mParentAllocator;
	char*       mMemory;
	uint32      mMemorySize;
	uint32      mUsedMemory;
	Mutex*     mMutex;

public:
	StackAllocator(uint32 size, IAllocator* parentAllocator = NULL);
	~StackAllocator();

	void* alloc(uint32 bytes);
	void* realloc(void* ptr, uint32 bytes);
	void free(void* ptr);
	const char* getName() const { return "stack alloc"; }
};

CLOSE_O2_NAMESPACE

#endif //STACK_ALLOCATOR_H