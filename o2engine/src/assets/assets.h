#ifndef ASSETS_H
#define ASSETS_H

#include "public.h"
#include "asset.h"
#include "util/file_system/file_info.h"

OPEN_O2_NAMESPACE

class asAsset;
class cLogStream;
class AssetBuildSystem;

class Assets
{
	friend class asAsset;
	friend class asFile;
	friend class asImage;
	friend class asXmlDoc;
	friend class abImageAssetInfo;
	friend class AssetBuildSystem;

public:
	typedef array<asAsset*> AssetsArr;

protected:
	AssetsInfosArr    mAssetsInfos;
	cLogStream*       mLog;
	AssetBuildSystem* mBuildSystem;
	string            mBuildedAssetsPath;

public:
	Assets();
	~Assets();

	string getAssetSourceFullPath(const string& path);
	string getAssetFullPath(const string& path);
	string getAssetRealPath(const string& path);
	FileLocation getAssetFileLocation(const string& path);

	template<typename _asType>
	_asType loadAsset(const string& path);

	template<typename _asType>
	_asType loadAsset(const FileLocation& location);
	
	template<typename _asType>
	_asType createAsset();
	
	template<typename _asType>
	void saveAsset(_asType& asset, const string& path);
	
	template<typename _asType>
	void removeAsset(_asType& asset);

	void rebuildAssets(bool forcible = false);

protected:
	void loadBuildedAssetsInfo();
	uint32 generateFileId() const;
};

template<typename _asType>
_asType Assets::createAsset()
{
	return _asType();
}

template<typename _asType>
void Assets::removeAsset(_asType& asset)
{
	fileSystem()->deleteFile(mBuildedAssetsPath + "/" + asset.getPath());
	rebuildAssets();
}

template<typename _asType>
_asType Assets::loadAsset(const string& path)
{
	_asType res(path);
	return res;
}

template<typename _asType>
void Assets::saveAsset(_asType& asset, const string& path)
{
	asset.save(path);
}

CLOSE_O2_NAMESPACE

#endif // ASSETS_H
