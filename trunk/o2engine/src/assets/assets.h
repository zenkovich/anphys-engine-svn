#ifndef ASSETS_H
#define ASSETS_H

#include "public.h"
#include "asset.h"

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

public:
	typedef array<asAssetInfo> AssetsInfosArr;
	typedef array<asAsset*> AssetsArr;

protected:
	AssetsInfosArr    mAssetsInfos;
	AssetsArr         mLoadedAssets;
	AssetsArr         mUnusedAssets;
	cLogStream*       mLog;
	AssetBuildSystem* mBuildSystem;

public:
	Assets();
	~Assets();

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

	void rebuildAssets(bool forcible = false);

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

	rebuildAssets();
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
