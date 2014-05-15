#include "smart_ptrs.h"

#include "log.h"

OPEN_O2_NAMESPACE
	

void xlog(const char* res)
{
	gLog->error(res);
}

CLOSE_O2_NAMESPACE