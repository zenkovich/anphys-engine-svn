#ifndef FILE_INFO_H
#define FILE_INFO_H

#include "public.h"
#include "file.h"
#include "util/time_utils.h"

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
};

class cPathInfo 
{
public:
	typedef vector<cFileInfo> FilesVec;
	typedef vector<cPathInfo> PathsVec;

	string   mPath;
	FilesVec mFiles;
	PathsVec mPaths;
};

CLOSE_O2_NAMESPACE

#endif //FILE_INFO_H
