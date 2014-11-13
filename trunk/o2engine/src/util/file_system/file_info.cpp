#include "file_info.h"

OPEN_O2_NAMESPACE
	
std::map<string, cSerializable*> gSerializeTypesContainer::mSamples;
SERIALIZE_METHOD_IMPL(cFileLocation)
{
	SERIALIZE_ID(mPath, "path");
	SERIALIZE_ID(mId, "id");
	return true;
}

bool cFileLocation::operator==(const cFileLocation& v) const
{
	if (mId != 0)
		return mId == v.mId;

	return mPath == v.mPath;
}

bool cFileLocation::operator!=(const cFileLocation& v) const
{
	return !(v == *this);
}


bool cFileInfo::operator==(const cFileInfo& fi) const
{
	return mPath == fi.mPath && mFileType == fi.mFileType && mCreatedDate == fi.mCreatedDate &&
		mAccessDate == fi.mAccessDate && mEditDate == fi.mEditDate && mSize == fi.mSize;
}


bool cPathInfo::operator==(const cPathInfo& pi) const
{
	return mPath == pi.mPath;
}

bool cPathInfo::isFileExist(const string& path)
{
	foreach(FilesArr, mFiles, fileIt)
		if (fileIt->mPath == path)
			return true;

	foreach(PathsArr, mPaths, pathIt)
		if (pathIt->isFileExist(path))
			return true;

	return false;
}

CLOSE_O2_NAMESPACE