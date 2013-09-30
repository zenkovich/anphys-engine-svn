#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class cLinearAllocator:public IAllocator
{
	IAllocator*  mParentAllocator;
	char*        mMemory;
	unsigned int mMemorySize;
	unsigned int mUsedMemory;

public:
	cLinearAllocator(unsigned int size, IAllocator* parentAllocator = NULL);
	~cLinearAllocator();

	void* alloc(uint32 bytes);
	void* realloc(void* ptr, uint32 bytes);
	void free(void* ptr);
	const char* getName() const { return "linear alloc"; }
};

CLOSE_O2_NAMESPACE

#endif //LINEAR_ALLOCATOR_H