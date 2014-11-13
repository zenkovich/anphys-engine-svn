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

	bool operator==(const cFileInfo& fi) const;
};

class cPathInfo 
{
public:
	typedef array<cFileInfo> FilesArr;
	typedef array<cPathInfo> PathsArr;

	string   mPath;
	FilesArr mFiles;
	PathsArr mPaths;

	bool operator==(const cPathInfo& pi) const;

	bool isFileExist(const string& path);
};

class cFileLocation: public cSerializable
{
public:
	string mPath;
	uint32 mId;

	cFileLocation(const string& path = "", uint32 id = 0):mPath(path), mId(id) {}

	bool operator==(const cFileLocation& v) const;
	bool operator!=(const cFileLocation& v) const;

	SERIALIZBLE_METHODS(cFileLocation);
};

CLOSE_O2_NAMESPACE

#endif //FILE_INFO_H
