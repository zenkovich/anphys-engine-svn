#ifndef BUILD_SYSTEM_H
#define BUILD_SYSTEM_H

#include "public.h"

#include "util/singleton.h"
#include "util/file_system/file.h"
#include "util/time_utils.h"
#include "util/serialize_util.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE
	
class cBuildConfig;
class cBuildInfo;

class cBuildSystem: public cSingleton<cBuildSystem>
{
public:
	struct FileMeta: public cSerializable
	{
		enum Type { MT_FOLDER = 0, MT_FILE, MT_IMAGE };

		string   mPath;
		Type     mType;
		int      mMetaId;
		bool     mBuildIncluded;
		uint32   mSize;
		WideTime mWritedTime;

		SERIALIZBLE_METHODS(FileMeta);
	};
	typedef vector<FileMeta*> FilesMetaVec;

	struct AssetChangesInfo
	{
		FilesMetaVec mNewFiles;
		FilesMetaVec mRemovedFiles;
		FilesMetaVec mMovedFiles;
		FilesMetaVec mChangedFiles;
	};

	typedef vector<cBuildConfig*> BuildConfigsVec;

protected:
	string          mProjectName;
	string          mProjectPath;
	BuildConfigsVec mBuildConfigs;
	cBuildConfig*   mActiveBuildConfig;
	cBuildInfo*     mBuildInfo;
	uint32          mLastMetaId;
	bool            mReady;

public:
	cBuildSystem(const string& projectPath);
	~cBuildSystem();

	bool loadProject(const string& projectPath);
	void createEmptyProject(const string& projName, const string& projPath);
	void createEmptyBuildConfig(const string& buildName);
	void setActiveBuildConfig(const string& name);
	void cleanUpBuildedAssest();
	void rebuildAssets(bool forcible = false);
	void saveConfig();

	string getBuildAssetsPath() const;
	string getAssetsPath() const;

private:
	void loadBuildInfo(bool errors = false);

	void gatherAssetsChanges(AssetChangesInfo& assetChangesInfo);
	void gatherAssetsFilesMeta(FilesMetaVec& filesMeta);
	void gatherAssetsFilesMetaFromFolder(cPathInfo& pathInfo, FilesMetaVec& filesMeta);
	FileMeta* createFileMetaFromFileInfo(const cFileInfo& fileInfo);

	void loadFileMeta(FileMeta* meta);
	void createFileMeta(FileMeta* meta);

	FileMeta* createFileMetaFromPathInfo(const cPathInfo& pathinfo);

	void updateBuildConfig();
	void updateBuildConfigPath(cPathInfo pathInfo);
	void copyNonBuildingFiles();
	void processAssetCopying(FileMeta* fileMeta, const string& assetsPath, const string& dataPath);

	void saveBuildInfo();
};

CLOSE_O2_NAMESPACE

#endif //BUILD_SYSTEM_H
