#ifndef BUILD_INFO_H
#define BUILD_INFO_H

#include "public.h"
#include "util/serialize_util.h"
#include "atlas_info.h"
#include "build_system.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

class cBuildInfo: public cSerializable
{
public:
	typedef vector<cImageAtlasInfo> AtlasesVec;
	typedef cBuildSystem::FilesMetaVec FilesMetaVec;

	FilesMetaVec mFilesMeta;
	AtlasesVec   mAtlases;

	cBuildInfo();
	~cBuildInfo();

	void addFile(cBuildSystem::FileMeta* meta);
	void removeFile(cBuildSystem::FileMeta* meta);
	cBuildSystem::FileMeta* findFile(int id);

	SERIALIZBLE_METHODS(cBuildInfo);
};

CLOSE_O2_NAMESPACE

#endif // BUILD_INFO_H
