#include "shared.h"

#include "log.h"
#include "assert.h"

OPEN_O2_NAMESPACE
	

void SharedsErrorOut(const char* res, bool asserting /*= true*/)
{
	if (asserting)
	{
		assert(false, res);
	}
	else
	{
		gLog->error(res);
	}
}

void SharedsRelease(void* ptr)
{
	delete ptr;
}

CLOSE_O2_NAMESPACE