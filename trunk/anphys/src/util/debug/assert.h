#ifndef ASSERT_H
#define ASSERT_H

#include "../log/log_system.h"
#include "../../engine/engine_options.h"
#include <cstdio>
#include <string>

#ifdef PLATFORM_WIN32

#define assert(x, desc) \
	if (!(x)) \
	{ \
		errorMessage(desc, __FILE__, __LINE__); \
		_asm{ int 3 };\
	}

#endif //PLATFORM_WIN32

void errorMessage(const std::string& desc, const std::string& file, long line);

#endif //ASSERT_H