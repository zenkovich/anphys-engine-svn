#include "build_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(cBuildInfo)
{
	SERIALIZE_ARR_ID(mFilesMeta, mFilesMeta.size(), "filesMeta");
	SERIALIZE_ARR_ID(mAtlases, mAtlases.size(), "atlases");
}

CLOSE_O2_NAMESPACE