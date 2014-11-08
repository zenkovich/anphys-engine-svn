#include "assets.h"

#include "asset.h"

OPEN_O2_NAMESPACE

Assets::SourcePath::SourcePath(const string& path):
	mPath(path)
{
	cSerializer serilizer;
	if (!serilizer.load(path + "/filesInfo.xml"))
	{
		logError("Failed to add source path to assets: can't load files info file. %s", path.c_str());
		return;
	}

	serilizer.serialize(mFiles, "files");
}

string Assets::SourcePath::getPath() const
{
	return mPath;
}

cFileLocation Assets::SourcePath::getFileLocation(const string& path) const
{
	foreach_const(FilesLocsArr, mFiles, file)
		if (file->mPath == path)
			return *file;

	return cFileLocation();
}

cFileLocation Assets::SourcePath::getFileLocation(uint32 id) const
{
	foreach_const(FilesLocsArr, mFiles, file)
		if (file->mId== id)
			return *file;

	return cFileLocation();
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

bool Assets::saveAsset(asAsset* asset, const string& path)
{

}

CLOSE_O2_NAMESPACE