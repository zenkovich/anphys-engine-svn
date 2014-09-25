#ifndef O2_ASSERT_H
#define O2_ASSERT_H

#include "public_namespace.h"
OPEN_O2_NAMESPACE

#ifdef assert
#undef assert
#endif

/** Outing assert with description, if x is false. */
#define o2assert(x, desc) \
	if (!(x)) \
	{ \
		errorMessage(desc, __FILE__, __LINE__); \
		_asm{ int 3 }; \
	}

void errorMessage(const char* desc, const char* file, long line);

CLOSE_O2_NAMESPACE

#endif //ASSERT_H