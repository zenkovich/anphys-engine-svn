#ifndef IALLOCATOR_H
#define IALLOCATOR_H

#include "public.h"

OPEN_O2_NAMESPACE
	
class IAllocator
{
public:
	virtual void* alloc(uint32 bytes) { return 0; }
	virtual void* realloc(void* ptr, uint32 bytes) { return 0; }

	virtual void free(void* ptr) {}

	virtual const char* getName() const { return "unknown"; }

	void* allocs(uint32 bytes, const char* source, unsigned int line);
	void* reallocs(void* ptr, uint32 bytes, const char* source, unsigned int line);
	void frees(void* ptr);
};

CLOSE_O2_NAMESPACE

#endif //IALLOCATOR_H