#include "build_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(cBuildInfo)
{
	SERIALIZE_ID(mFilesMeta, "filesMeta");
	SERIALIZE_ID(mAtlases, "atlases");

	return true;
}



CLOSE_O2_NAMESPACE