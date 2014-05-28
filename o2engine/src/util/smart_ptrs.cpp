#include "smart_ptrs.h"

#include "log.h"
#include "assert.h"

OPEN_O2_NAMESPACE
	

void xlog(const char* res)
{
	assert(false, res);
	gLog->error(res);
}

void xrelease(void* data)
{
	delete data;
}

CLOSE_O2_NAMESPACE