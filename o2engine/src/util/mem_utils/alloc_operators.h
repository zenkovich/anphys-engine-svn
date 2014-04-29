#ifndef ALLOC_OPERATORS_H
#define ALLOC_OPERATORS_H

#include "public.h"

/* Define overloaded new/delete operators if basic memory allocator was specified. */
#ifdef BASIC_MEMORY_ALLOCATOR 

	void* operator new(uint32 size);
	void* operator new[](uint32 size);

	void operator delete(void* ptr);
	void operator delete[](void* ptr);

#endif //BASIC_MEMORY_ALLOCATOR

/* Define overloaded and tracing new/delete operators if memory trace enabled. */
#ifdef MEMORY_TRACE

	void* operator new(uint32 size, const char* location, int line);
	void* operator new[](uint32 size, const char* location, int line);

	void operator delete(void* ptr, const char* location, int line);
	void operator delete[](void* ptr, const char* location, int line);

	/* Basic engine allocation comand, what tracing source of allocation. */
	#define mnew new (__FILE__, __LINE__) 
	
	/** Alloc memory from allocator with specified size. Tracing location, if enabled. */
	#define ALLOC(allocator, size) allocator->allocs(size, __FILE__, __LINE__); 

	/** Realloc memory from allocator with specified size. Tracing location, if enabled. */
	#define REALLOC(allocator, mem_ptr, size) allocator->reallocs(mem_ptr, size, __FILE__, __LINE__); 

	/** Frees memory from allocator with specified size. Tracing location, if enabled. */
	#define FREE(allocator, mem_ptr) allocator->frees(mem_ptr); 

#else
	
	/** Alloc memory from allocator with specified size. Tracing location, if enabled. */
	#define mnew new
	
	/** Alloc memory from allocator with specified size. Tracing location, if enabled. */
	#define ALLOC(allocator, size) allocator->alloc(size); 

	/** Realloc memory from allocator with specified size. Tracing location, if enabled. */
	#define REALLOC(allocator, mem_ptr, size) allocator->realloc(mem_ptr, size); 

	/** Frees memory from allocator with specified size. Tracing location, if enabled. */
	#define FREE(allocator, mem_ptr) allocator->free(mem_ptr); 

#endif //MEMORY_TRACE

OPEN_O2_NAMESPACE

#ifndef DEBUG_POINTERS
/** Safe release object. */
#	define safe_release(obj) { if (obj != 0) delete obj; }

	/** Safe release array object. */
#	define safe_release_arr(obj) { if (obj != 0) delete[] obj; }
#else

#define safe_release(obj) \
{ \
	void* obj_ptr = safe_release_(obj); \
	if(obj_ptr != 0)                    \
		delete obj_ptr;                 \
}

#define safe_release_arr(obj) \
{ \
	void* obj_ptr = safe_release_arr_(obj); \
	if(obj_ptr != 0)                        \
		delete[] obj_ptr;                   \
}


	template<typename T>
	void* safe_release_(T* object)
	{
		return object;
	}

	template<typename T>
	void* safe_release_arr_(T* object)
	{
		return object;
	}

#endif

CLOSE_O2_NAMESPACE


#endif //ALLOC_OPERATORS_H