#ifndef FILE_INFO_H
#define FILE_INFO_H

#include "public.h"
#include "file.h"
#include "util/time_utils.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class FileInfo 
{
public:
	string           mPath;
	FileType::value mFileType;
	WideTime         mCreatedDate;
	WideTime         mAccessDate;
	WideTime         mEditDate;
	uint32           mSize;
	uint64           mCheckSumm;

	bool operator==(const FileInfo& fi) const;
};

class PathInfo 
{
public:
	typedef array<FileInfo> FilesArr;
	typedef array<PathInfo> PathsArr;

	string   mPath;
	FilesArr mFiles;
	PathsArr mPaths;

	bool operator==(const PathInfo& pi) const;

	bool isFileExist(const string& path);
	void clampPathNames();

protected:
	void processPathNamesClamping(int charCount);
};

class FileLocation: public Serializable
{
public:
	string mPath;
	uint32 mId;

	FileLocation(const string& path = "", uint32 id = 0):mPath(path), mId(id) {}

	bool operator==(const FileLocation& v) const;
	bool operator!=(const FileLocation& v) const;

	SERIALIZBLE_METHODS(FileLocation);
};
typedef array<FileLocation> FileLocationsArr;

CLOSE_O2_NAMESPACE

#endif //FILE_INFO_H
