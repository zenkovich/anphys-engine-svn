#ifndef ATLAS_INFO_H
#define ATLAS_INFO_H

#include "public.h"
#include "build_config.h"

OPEN_O2_NAMESPACE

class cImageAtlasInfo
{
public: 
	typedef vector<cBuildSystem::ImageFileMeta> ImagesMetaVec;

	string        mName;
	vec2f         mMaxSize;
	ImagesMetaVec mImages;
};

CLOSE_O2_NAMESPACE

#endif // ATLAS_INFO_H
