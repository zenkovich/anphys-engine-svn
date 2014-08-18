#include "atlas_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(cImageAtlasInfo)
{
	SERIALIZE_ID(mName, "name");
	SERIALIZE_ID(mMaxSize, "maxSize");

	SERIALIZE_ARR_ID(mImages, mImages.size(), "images");
}

CLOSE_O2_NAMESPACE