#ifndef BUILD_INFO_H
#define BUILD_INFO_H

#include "public.h"
#include "util/serialize_util.h"
#include "build_system.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

class cImageAtlasInfo;

class cBuildInfo: public cSerializable
{
public:
	typedef vector<cImageAtlasInfo*> AtlasesVec;
	typedef cBuildSystem::FilesMetaVec FilesMetaVec;

	FilesMetaVec     mFilesMeta;
	AtlasesVec       mAtlases;
	cImageAtlasInfo* mBasicAtlas;

	cBuildInfo();
	virtual ~cBuildInfo();

	void addFile(cBuildSystem::FileMeta* meta);
	void removeFile(cBuildSystem::FileMeta* meta);
	cBuildSystem::FileMeta* findFile(uint32 id);
	cBuildSystem::FileMeta* findFile(const cFileLocation& location);

	cImageAtlasInfo* addAtlas(const string& name, const vec2f& maxSize, const string& attachingPath = "");
	cImageAtlasInfo* getAtlas(const string& name) const;
	void removeAtlas(const string& name);

	SERIALIZBLE_METHODS(cBuildInfo);
};

CLOSE_O2_NAMESPACE

#endif // BUILD_INFO_H
