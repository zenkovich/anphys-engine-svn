#ifndef BUILD_SYSTEM_H
#define BUILD_SYSTEM_H

#include "public.h"

#include "util/singleton.h"
#include "util/file_system/file.h"
#include "util/time_utils.h"
#include "util/serialize_util.h"
#include "util/file_system/file_system.h"
#include "build_config.h"
#include "atlas_info.h"

OPEN_O2_NAMESPACE	

class cBuildSystem: public cSingleton<cBuildSystem>
{
	friend struct cNonBuildFilesBuildStage;
	friend struct cAtlasesBuildingStage;

public:
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

	cImageAtlasInfo* createImageAtlas(const string& name, const vec2f& maxSize, cBuildPathInfo* attachingPath = NULL);
	void removeAtlas(const string& name);
	cImageAtlasInfo* getAtlas(const string& name);

private:
	void loadBuildInfo(bool errors = false);

	void updateBuildConfig();
	void gatherAssetsFileInfos(BuildFileInfoVec& filesMeta);
	void gatherAssetsFileInfosFromFolder(cPathInfo& pathInfo, BuildFileInfoVec& filesMeta);
	cBuildFileInfo* createBuildFileInfo(const cFileInfo& fileInfo);
	cBuildFileInfo* createBuildFileMeta(const cPathInfo& pathinfo);
	void loadFileMeta(cBuildFileInfo* info, const string& pathPrefix = "");
	void createFileMeta(cBuildFileInfo* info, const string& pathPrefix = "");
	void processBuildStages();
	uint32 genNewMetaId() const;

	void saveBuildInfo();
};

CLOSE_O2_NAMESPACE

#endif //BUILD_SYSTEM_H
