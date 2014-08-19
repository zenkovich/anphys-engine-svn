#include "atlas_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(cImageAtlasInfo)
{
	SERIALIZE_ID(mName, "name");
	SERIALIZE_ID(mMaxSize, "maxSize");
	SERIALIZE_ID(mImages, "images");

	return true;
}

CLOSE_O2_NAMESPACE