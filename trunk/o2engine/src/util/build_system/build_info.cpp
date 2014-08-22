#include "build_info.h"

OPEN_O2_NAMESPACE
	

cBuildInfo::cBuildInfo()
{
}

cBuildInfo::~cBuildInfo()
{
	FOREACH(FilesMetaVec, mFilesMeta, meta)
		safe_release(*meta);
	mFilesMeta.clear();
}

void cBuildInfo::addFile(cBuildSystem::FileMeta* meta)
{
	mFilesMeta.push_back(meta);
}

void cBuildInfo::removeFile(cBuildSystem::FileMeta* meta)
{
	FOREACH(FilesMetaVec, mFilesMeta, metaIt)
	{
		if (meta->mMetaId == (*metaIt)->mMetaId)
		{
			mFilesMeta.erase(metaIt);
			return;
		}
	}
}

cBuildSystem::FileMeta* cBuildInfo::findFile(int id)
{
	FOREACH(FilesMetaVec, mFilesMeta, metaIt)
	{
		if (id == (*metaIt)->mMetaId)
			return *metaIt;
	}

	return NULL;
}

SERIALIZE_METHOD_IMPL(cBuildInfo)
{
	SERIALIZE_ID(mFilesMeta, "filesMeta");
	SERIALIZE_ID(mAtlases, "atlases");

	return true;
}

CLOSE_O2_NAMESPACE