#ifndef TEST_ALLOCATOR_H
#define TEST_ALLOCATOR_H

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class Mutex;

class TestAllocator:public IAllocator
{
	IAllocator* mParentAllocator;
	char*       mMemory;
	uint32      mMemorySize;

	//cMutex*     mMutex;

public:
	TestAllocator(uint32 size, IAllocator* parentAllocator = NULL);
	~TestAllocator();

	void* alloc(uint32 bytes);
	void* realloc(void* ptr, uint32 bytes);
	void free(void* ptr);
	const char* getName() const { return "test alloc"; }

	void dump();

protected:
	void* putBlock(void* ptr, uint32 size, bool clear);
};

CLOSE_O2_NAMESPACE

#endif //TEST_ALLOCATOR_H