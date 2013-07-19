#ifndef ASSERT_H
#define ASSERT_H

#include "public.h"
OPEN_O2_NAMESPACE

#define assert(x, desc) \
	if (!(x)) \
	{ \
		errorMessage(desc, __FILE__, __LINE__); \
	}

#endif //PLATFORM_WIN32

void errorMessage(const char* desc, const char* file, long line);

CLOSE_O2_NAMESPACE

#endif //ASSERT_H