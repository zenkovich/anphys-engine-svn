#ifndef ASSETS_H
#define ASSETS_H

#include "public.h"
#include "util/file_system/file_info.h"

OPEN_O2_NAMESPACE

class asAsset;
class cLogStream;

class Assets
{
	friend class asAsset;

public:
	typedef array<cFileLocation> FilesLocsArr;
	typedef array<asAsset*> AssetsArr;

	class SourcePath
	{
		string       mPath;
		FilesLocsArr mFiles;

	public:
		SourcePath();
		SourcePath(const string& path);

		string getPath() const;
		bool getFileLocation(const string& path, cFileLocation& location) const;
		bool getFileLocation(uint32 id, cFileLocation& location) const;

		bool operator==(const SourcePath& other);
	};
	typedef array<SourcePath> SourcePathsArr;

protected:
	SourcePathsArr mSourcePaths;
	AssetsArr      mLoadedAssets;
	AssetsArr      mUnusedAssets;
	cLogStream*    mLog;

public:
	Assets();
	~Assets();

	void addAssetsPath(const string& path);

	string getAssetsRealPath(const string& path);
	cFileLocation getAssetFileLocation(const string& path);

	template<typename _asType>
	_asType loadAsset(const string& path);
	
	template<typename _asType>
	_asType createAsset();
	
	template<typename _asType>
	void saveAsset(_asType& asset, const string& path);
	
	template<typename _asType>
	void removeAsset(_asType& asset);

	void saveLoadedAssets();

protected:
	uint32 generateFileId() const;

	void assetUnused(asAsset* asset);
};

template<typename _asType>
_asType Assets::createAsset()
{
	return _asType();
}

template<typename _asType>
void Assets::removeAsset(_asType& asset)
{
	if (asset.mObject->getRefCount() > 2)
		logWarning("Possible using removed asset: %s", asset.getPath().c_str());

	mLoadedAssets.remove(asset.mObject);
	fileSystem()->deleteFile(asset.getPath());

	//check assets rebuilding
}

template<typename _asType>
_asType Assets::loadAsset(const string& path)
{
	_asType res(path);
	mLoadedAssets.add(res.mObject);
	return res;
}

template<typename _asType>
void Assets::saveAsset(_asType& asset, const string& path)
{
	asset.save(path);
}

CLOSE_O2_NAMESPACE

#endif // ASSETS_H
