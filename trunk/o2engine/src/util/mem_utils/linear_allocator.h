#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class Mutex;

/** Linear allocator. */
class LinearAllocator:public IAllocator
{
	IAllocator* mParentAllocator; /**< Parent allocator. This allocator allocates memory from parent allocator. 
								    *  If parent allocator not specified, using system allocator. */

	char*       mMemory;          /**< Memory ptr. */
	uint32      mMemorySize;      /**< Size of memory. */
	uint32      mUsedMemory;      /**< Size of used memory. */
	Mutex*     mMutex;           /**< Mutex. */

public:
	/** ctor. */
	LinearAllocator(uint32 size, IAllocator* parentAllocator = NULL);

	/** dtor. */
	~LinearAllocator();

	/** Allocates bytes memory. */
	void* alloc(uint32 bytes);

	/** Frees memory by pointer. */
	void free(void* ptr);

	/** Returns allocator identificator. */
	const char* getName() const { return "linear alloc"; }
};

CLOSE_O2_NAMESPACE

#endif //LINEAR_ALLOCATOR_H