#include "file_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(FileLocation)
{
	SERIALIZE_ID(mPath, "path");
	SERIALIZE_ID(mId, "id");
	return true;
}

bool FileLocation::operator==(const FileLocation& v) const
{
	if (mId != 0)
		return mId == v.mId;

	return mPath == v.mPath;
}

bool FileLocation::operator!=(const FileLocation& v) const
{
	return !(v == *this);
}


bool FileInfo::operator==(const FileInfo& fi) const
{
	return mPath == fi.mPath && mFileType == fi.mFileType && mCreatedDate == fi.mCreatedDate &&
		mAccessDate == fi.mAccessDate && mEditDate == fi.mEditDate && mSize == fi.mSize;
}


bool PathInfo::operator==(const PathInfo& pi) const
{
	return mPath == pi.mPath;
}

bool PathInfo::isFileExist(const string& path)
{
	foreach(FilesArr, mFiles, fileIt)
		if (fileIt->mPath == path)
			return true;

	foreach(PathsArr, mPaths, pathIt)
		if (pathIt->isFileExist(path))
			return true;

	return false;
}

void PathInfo::clampPathNames()
{
	processPathNamesClamping(mPath.length() + 1);
}

void PathInfo::processPathNamesClamping(int charCount)
{
	mPath = mPath.substr(min(charCount, (int)mPath.length()), max((int)mPath.length() - charCount, 0));

	foreach(FilesArr, mFiles, fileIt)
		fileIt->mPath = fileIt->mPath.substr(min(charCount, (int)fileIt->mPath.length()), 
		                                     max((int)fileIt->mPath.length() - charCount, 0));

	foreach(PathsArr, mPaths, pathIt)
		pathIt->processPathNamesClamping(charCount);
}

CLOSE_O2_NAMESPACE