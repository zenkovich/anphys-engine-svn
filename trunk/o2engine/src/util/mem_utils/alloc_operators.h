#ifndef ALLOC_OPERATORS_H
#define ALLOC_OPERATORS_H

#include "public.h"

#ifdef BASIC_MEMORY_ALLOCATOR

	void* operator new(uint32 size);
	void* operator new[](uint32 size);

	void operator delete(void* ptr);
	void operator delete[](void* ptr);

#endif

#ifdef MEMORY_TRACE

	void* operator new(uint32 size, const char* location, int line);
	void* operator new[](uint32 size, const char* location, int line);

	void operator delete(void* ptr, const char* location, int line);
	void operator delete[](void* ptr, const char* location, int line);

	#define mnew new (__FILE__, __LINE__) 
	
	/** Alloc memory from allocator with specified size. Tracing location, if enabled. */
	#define ALLOC(allocator, size) allocator->allocs(size, __FILE__, __LINE__); 

	/** Realloc memory from allocator with specified size. Tracing location, if enabled. */
	#define REALLOC(allocator, mem_ptr, size) allocator->reallocs(mem_ptr, size, __FILE__, __LINE__); 

	/** Frees memory from allocator with specified size. Tracing location, if enabled. */
	#define FREE(allocator, mem_ptr) allocator->frees(mem_ptr); 

#else

	#define mnew new
	
	/** Alloc memory from allocator with specified size. Tracing location, if enabled. */
	#define ALLOC(allocator, size) allocator->alloc(size); 

	/** Realloc memory from allocator with specified size. Tracing location, if enabled. */
	#define REALLOC(allocator, mem_ptr, size) allocator->realloc(mem_ptr, size); 

	/** Frees memory from allocator with specified size. Tracing location, if enabled. */
	#define FREE(allocator, mem_ptr) allocator->free(mem_ptr); 

#endif //MEMORY_TRACE


#endif //ALLOC_OPERATORS_H