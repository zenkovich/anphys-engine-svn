#ifndef ATLAS_INFO_H
#define ATLAS_INFO_H

#include "public.h"
#include "util/serialize_util.h"
#include "build_system.h"

OPEN_O2_NAMESPACE

class cImageAtlasInfo: public cSerializable
{
public: 
	typedef vector<cBuildSystem::ImageFileMeta> ImagesMetaVec;

	string        mName;
	vec2f         mMaxSize;
	ImagesMetaVec mImages;

	SERIALIZBLE_METHODS(cImageAtlasInfo);
};

CLOSE_O2_NAMESPACE

#endif // ATLAS_INFO_H
