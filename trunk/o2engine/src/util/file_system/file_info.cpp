#include "file_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(cFileLocation)
{
	SERIALIZE_ID(mPath, "path");
	SERIALIZE_ID(mId, "id");
	return true;
}

CLOSE_O2_NAMESPACE