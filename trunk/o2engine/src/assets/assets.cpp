#include "assets.h"

#include "asset.h"
#include "util/string.h"

OPEN_O2_NAMESPACE


Assets::Assets()
{
	string buildedAssetsPath = "../assets/";
	string assetsInfoFilePath = buildedAssetsPath + "../assetsInfo.xml";

	mLog = mnew cLogStream("assets");
	gLog->bindStream(mLog);

	cSerializer serializer;
	if (serializer.load(assetsInfoFilePath))
	{
		serializer.serialize(mFilesLocs, "files");
	}
}

Assets::~Assets()
{
	safe_release(mLog);
}

string Assets::getAssetsRealPath( const string& path )
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

		foreach(FilesLocsArr, mFilesLocs, fileLocIt)
		{
			if (fileLocIt->mId == fileId)
			{
				convertedPath.replace(convertedPath.begin(),    convertedPath.begin() + idEndIdx + 1,
					                  fileLocIt->mPath.begin(), fileLocIt->mPath.end());

				break;
			}
		}
	}

	return convertedPath;
}

cFileLocation Assets::getAssetFileLocation(const string& path)
{
	string convertedPath = getAssetsRealPath(path);

	foreach(FilesLocsArr, mFilesLocs, fileIt)
		if (fileIt->mPath == path)
			return *fileIt;

	return cFileLocation();
}

void Assets::saveLoadedAssets()
{
	foreach(AssetsArr, mLoadedAssets, asset)
		(*asset)->saveData();

	//check assets rebuilding
	rebuildAssets();
}

uint32 Assets::generateFileId() const
{
	return rand()%(UINT_MAX - 1) + 1;
}

void Assets::assetUnused(asAsset* asset)
{
	mLoadedAssets.remove(asset);
	asset->saveData();
	mUnusedAssets.add(asset);

	//check assets rebuilding
	rebuildAssets();
}

void Assets::rebuildAssets(bool forcible /*= false*/)
{

}

CLOSE_O2_NAMESPACE