#include "assets.h"

#include "asset.h"
#include "util/string.h"
#include "asset_build_system/asset_build_system.h"

OPEN_O2_NAMESPACE


Assets::Assets()
{
	mLog = mnew cLogStream("assets");
	gLog->bindStream(mLog);

	mBuildSystem = mnew AssetBuildSystem(this);

	if (ASSETS_PREBUILDING_ENABLE)
		rebuildAssets();
	else
		loadBuildedAssetsInfo();
}

Assets::~Assets()
{
	safe_release(mLog);
	safe_release(mBuildSystem);
}

string Assets::getAssetSourceFullPath(const string& path)
{
	return ASSETS_PATH + getAssetRealPath(path);
}

string Assets::getAssetFullPath(const string& path)
{
	return ASSETS_BUILDED_PATH + getAssetRealPath(path);
}

string Assets::getAssetRealPath( const string& path )
{
	string convertedPath = path;

	if (convertedPath == "")
		return convertedPath;

	uint32 fileId = 0;
	if (convertedPath[0] == '@')
	{
		int idEndIdx = convertedPath.find('@', 1);
		string fileIdStr = convertedPath.substr(1, idEndIdx - 1);
		fileId = toInt(fileIdStr);

		foreach(AssetsInfosArr, mAssetsInfos, fileLocIt)
		{
			if (fileLocIt->mLocation.mId == fileId)
			{
				convertedPath.replace(convertedPath.begin(),              convertedPath.begin() + idEndIdx + 1,
					                  fileLocIt->mLocation.mPath.begin(), fileLocIt->mLocation.mPath.end());

				break;
			}
		}
	}

	return convertedPath;
}

cFileLocation Assets::getAssetFileLocation(const string& path)
{
	string convertedPath = getAssetRealPath(path);

	foreach(AssetsInfosArr, mAssetsInfos, fileIt)
		if (fileIt->mLocation.mPath == convertedPath)
			return fileIt->mLocation;

	return cFileLocation();
}

uint32 Assets::generateFileId() const
{
	return rand()%(UINT_MAX - 1) + 1;
}

void Assets::rebuildAssets(bool forcible /*= false*/)
{
	mBuildSystem->rebuildAssets(forcible);
	loadBuildedAssetsInfo();
}

void Assets::loadBuildedAssetsInfo()
{
	mAssetsInfos.clear();

	cSerializer serializer;
	if (serializer.load(ASSETS_INFO_FILE_PATH))
	{
		serializer.serialize(mAssetsInfos, "assets");
	}
}

CLOSE_O2_NAMESPACE