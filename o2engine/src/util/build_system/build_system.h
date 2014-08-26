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
	friend struct cNonBuildFilesBuildStage;

public:
	struct FileMeta: public cSerializable
	{
		enum Type { MT_FOLDER = 0, MT_FILE, MT_IMAGE };

		string   mPath;
		Type     mType;
		uint32   mMetaId;
		bool     mBuildIncluded;
		uint32   mSize;
		WideTime mWritedTime;

		SERIALIZBLE_METHODS(FileMeta);
		virtual FileMeta* clone() const;
	};
	typedef vector<FileMeta*> FilesMetaVec;

	struct ImageFileMeta: public FileMeta
	{
		string mAtlas;
		SERIALIZBLE_INHERITED_METHODS(ImageFileMeta, FileMeta);
		virtual FileMeta* clone() const;
	};

	struct PathMeta: public FileMeta
	{
		string mAttachedAtlas;
		SERIALIZBLE_INHERITED_METHODS(PathMeta, FileMeta);
		virtual FileMeta* clone() const;
	};

	struct AssetChangesInfo
	{
		FilesMetaVec mNewFiles;
		FilesMetaVec mRemovedFiles;
		FilesMetaVec mMovedFiles;
		FilesMetaVec mChangedFiles;

		AssetChangesInfo();
		~AssetChangesInfo();

		void clear();
	};

	struct IBuildStage
	{
		cBuildSystem* mBuildSystem;

		IBuildStage(cBuildSystem* buildSystem):mBuildSystem(buildSystem) {}
		virtual void process() = 0;
	};

	typedef vector<cBuildConfig*> BuildConfigsVec;
	typedef vector<IBuildStage*> BuildStagesVec;

protected:
	string           mProjectName;
	string           mProjectPath;
	BuildConfigsVec  mBuildConfigs;
	cBuildConfig*    mActiveBuildConfig;
	cBuildInfo*      mBuildInfo;
	AssetChangesInfo mAssetsChangesInfo;
	bool             mReady;

	BuildStagesVec   mBuildStages;

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

	void gatherAssetsChanges();
	void gatherAssetsFilesMeta(FilesMetaVec& filesMeta);
	void gatherAssetsFilesMetaFromFolder(cPathInfo& pathInfo, FilesMetaVec& filesMeta);
	FileMeta* createFileMetaFromFileInfo(const cFileInfo& fileInfo);
	FileMeta* createFileMetaFromPathInfo(const cPathInfo& pathinfo);
	void loadFileMeta(FileMeta* meta, const string& pathPrefix = "");
	void createFileMeta(FileMeta* meta, const string& pathPrefix = "");
	void processBuildStages();

	uint32 genNewMetaId() const;

	void saveBuildInfo();
};

CLOSE_O2_NAMESPACE

#endif //BUILD_SYSTEM_H
