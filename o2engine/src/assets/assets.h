#ifndef ASSETS_H
#define ASSETS_H

#include "public.h"
#include "util/file_system/file_info.h"

OPEN_O2_NAMESPACE

class asAsset;
class cLogStream;

class Assets
{
public:
	typedef array<cFileLocation> FilesLocsArr;

	class SourcePath
	{
		string       mPath;
		FilesLocsArr mFiles;

	public:
		SourcePath(const string& path);

		string getPath() const;
		bool getFileLocation(const string& path, cFileLocation& location) const;
		bool getFileLocation(uint32 id, cFileLocation& location) const;
	};
	typedef array<SourcePath> SourcePathsArr;

protected:
	SourcePathsArr mSourcePaths;
	cLogStream*    mLog;

public:
	Assets();
	~Assets();

	void addAssetsPath(const string& path);

	string getPathByFileFromFileId(const string& path);

	asAsset* loadAsset(const string& path);

	template<typename _asType>
	_asType* loadAsset(const string& path);

	bool saveAsset(asAsset* asset, const string& path);
};

template<typename _asType>
_asType* Assets::loadAsset(const string& path)
{	
	return mnew _asType(getPathByFileFromFileId(path));
}

CLOSE_O2_NAMESPACE

#endif // ASSETS_H
