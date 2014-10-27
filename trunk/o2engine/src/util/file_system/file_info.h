#ifndef FILE_INFO_H
#define FILE_INFO_H

#include "public.h"
#include "file.h"
#include "util/time_utils.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class cFileInfo 
{
public:
	string           mPath;
	cFileType::value mFileType;
	WideTime         mCreatedDate;
	WideTime         mAccessDate;
	WideTime         mEditDate;
	uint32           mSize;
	uint64           mCheckSumm;

	bool operator==(const cFileInfo& fi) const
	{
		return mPath == fi.mPath && mFileType == fi.mFileType && mCreatedDate == fi.mCreatedDate &&
			   mAccessDate == fi.mAccessDate && mEditDate == fi.mEditDate && mSize == fi.mSize;
	}
};

class cPathInfo 
{
public:
	typedef array<cFileInfo> FilesArr;
	typedef array<cPathInfo> PathsArr;

	string   mPath;
	FilesArr mFiles;
	PathsArr mPaths;

	bool operator==(const cPathInfo& pi) const
	{
		return mPath == pi.mPath;
	}
};

class cFileLocation: public cSerializable
{
public:
	string mPath;
	uint32 mId;

	cFileLocation(const string& path = "", uint32 id = 0):mPath(path), mId(id) {}

	bool operator==(const cFileLocation& v) const
	{ 
		if (mId != 0)
			return mId == v.mId;

		return mPath == v.mPath; 
	}

	bool operator!=(const cFileLocation& v) const
	{ 
		return !(v == *this);
	}

	SERIALIZBLE_METHODS(cFileLocation);
};

CLOSE_O2_NAMESPACE

#endif //FILE_INFO_H
