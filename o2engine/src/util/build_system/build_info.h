#ifndef BUILD_INFO_H
#define BUILD_INFO_H

#include "public.h"
#include "build_system.h"

OPEN_O2_NAMESPACE

class cBuildInfo 
{
public:
	typedef vector<cImageAtlasInfo> AtlasesVec;
	typedef cBuildSystem::FilesMetaVec FilesMetaVec;

	FilesMetaVec mFilesMeta;
	AtlasesVec   mAtlases;
};

CLOSE_O2_NAMESPACE

#endif // BUILD_INFO_H
