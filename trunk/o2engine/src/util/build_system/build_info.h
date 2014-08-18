#ifndef BUILD_INFO_H
#define BUILD_INFO_H

#include "public.h"
#include "util/serialization/serialization.h"
#include "atlas_info.h"
#include "build_system.h"

OPEN_O2_NAMESPACE

class cBuildInfo: public cSerializableObj
{
public:
	typedef vector<cImageAtlasInfo> AtlasesVec;
	typedef cBuildSystem::FilesMetaVec FilesMetaVec;

	FilesMetaVec mFilesMeta;
	AtlasesVec   mAtlases;

	SERIALIZE_METHOD_DECL();
};

CLOSE_O2_NAMESPACE

#endif // BUILD_INFO_H
