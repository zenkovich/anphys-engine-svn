#include "string.h"

#include <cstdarg>

OPEN_O2_NAMESPACE

std::string format( const std::string& str, ... )
{
	const uint32 bufsz = 1096;
	char buf[bufsz];

	va_list ap;
	va_start(ap, str);
	vsnprintf(buf, bufsz, str.c_str(), ap);
	va_end(ap);

	return std::string(buf);
}

CLOSE_O2_NAMESPACE