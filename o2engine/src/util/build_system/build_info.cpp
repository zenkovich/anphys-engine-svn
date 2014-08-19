#include "build_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(cBuildInfo)
{
	SERIALIZE_ID(mFilesMeta, "filesMeta");
	SERIALIZE_ID(mAtlases, "atlases");

	return true;
}

void cBuildInfo::addFile(const cFileInfo& fileInfo)
{
	cBuildSystem::FileMeta* newMeta = NULL;
	if (fileInfo.mFileType == cFileType::FT_IMAGE)
		newMeta = mnew cBuildSystem::ImageFileMeta();
	else
		newMeta = mnew cBuildSystem::FileMeta();

	newMeta->mPath = fileInfo.mPath;
	newMeta->mBuildIncluded = true;
	newMeta->mSize = fileInfo.mSize;
	newMeta->mWritedTime = fileInfo.mEditDate;

	mFilesMeta.push_back(newMeta);
}

CLOSE_O2_NAMESPACE