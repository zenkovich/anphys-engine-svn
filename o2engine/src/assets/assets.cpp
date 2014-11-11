#include "assets.h"

#include "asset.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

Assets::SourcePath::SourcePath(const string& path):
	mPath(path)
{
	cSerializer serilizer;
	if (!serilizer.load(path + "/filesInfo.xml"))
	{
		//logError("Failed to add source path to assets: can't load files info file. %s", path.c_str());
		return;
	}

	serilizer.serialize(mFiles, "files");
}

Assets::SourcePath::SourcePath()
{

}

string Assets::SourcePath::getPath() const
{
	return mPath;
}

bool Assets::SourcePath::getFileLocation(const string& path, cFileLocation& location) const
{
	foreach_const(FilesLocsArr, mFiles, file)
	{
		if (file->mPath == path)
		{
			location = *file;
			return true;
		}
	}

	return false;
}

bool Assets::SourcePath::getFileLocation(uint32 id, cFileLocation& location) const
{
	foreach_const(FilesLocsArr, mFiles, file)
	{
		if (file->mId == id)
		{
			location = *file;
			return true;
		}
	}

	return false;
}

bool Assets::SourcePath::operator==(const SourcePath& other)
{
	return mPath == other.mPath;
}


Assets::Assets()
{
	mLog = new cLogStream("assets");
	gLog->bindStream(mLog);
}

Assets::~Assets()
{
	safe_release(mLog);
}

void Assets::addAssetsPath(const string& path)
{
	mSourcePaths.add(SourcePath(path));
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

		foreach_back(SourcePathsArr, mSourcePaths, sourcePath)
		{
			cFileLocation fileLoc;
			if (sourcePath->getFileLocation(fileId, fileLoc))
			{
				convertedPath.replace(convertedPath.begin(), convertedPath.begin() + idEndIdx + 1,
					                  fileLoc.mPath.begin(), fileLoc.mPath.end());

				break;
			}
		}
	}

	return convertedPath;
}

cFileLocation Assets::getAssetFileLocation(const string& path)
{
	string convertedPath = getAssetsRealPath(path);

	foreach_back(SourcePathsArr, mSourcePaths, sourcePath)
	{
		cFileLocation fileLoc;
		if (sourcePath->getFileLocation(convertedPath, fileLoc))
			return fileLoc;
	}
}

void Assets::saveLoadedAssets()
{
	foreach(AssetsArr, mLoadedAssets, asset)
		(*asset)->saveData();

	//check assets rebuilding
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
}

CLOSE_O2_NAMESPACE