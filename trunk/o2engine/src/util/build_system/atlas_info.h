#ifndef ATLAS_INFO_H
#define ATLAS_INFO_H

#include "public.h"
#include "util/serialization/serialization.h"
#include "build_system.h"

OPEN_O2_NAMESPACE

class cImageAtlasInfo: public cSerializableObj
{
public: 
	typedef vector<cBuildSystem::ImageFileMeta> ImagesMetaVec;

	string        mName;
	vec2f         mMaxSize;
	ImagesMetaVec mImages;

	SERIALIZE_METHOD_DECL();
};

CLOSE_O2_NAMESPACE

#endif // ATLAS_INFO_H
