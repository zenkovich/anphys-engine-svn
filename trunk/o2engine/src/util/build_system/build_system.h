#ifndef BUILD_SYSTEM_H
#define BUILD_SYSTEM_H

#include "public.h"

#include "util/singleton.h"
#include "util/file_system/file.h"
#include "util/time_utils.h"
#include "util/serialize_util.h"
#include "util/file_system/file_system.h"
#include "build_config.h"

OPEN_O2_NAMESPACE	

class cImageAtlasInfo;

class cBuildSystem: public cSingleton<cBuildSystem>
{
	friend class cNonBuildFilesBuildStage;
	friend class cAtlasesBuildingStage;

public:
	struct IBuildStage
	{
		cBuildSystem* mBuildSystem;

		IBuildStage(cBuildSystem* buildSystem):mBuildSystem(buildSystem) {}
		virtual void process() = 0;
	};

	typedef array<cBuildConfig*> BuildConfigsArr;
	typedef array<IBuildStage*> BuildStagesArr;
	typedef array<cImageAtlasInfo*> AtlasesArr;

protected:
	string           mProjectName;
	string           mProjectPath;
	string           mAtlasesPath;
	BuildConfigsArr  mBuildConfigs;
	cBuildConfig*    mActiveBuildConfig;
	cBuildInfo*      mBuildInfo;
	cImageAtlasInfo* mBasicAtlas;

	bool             mReady;

	BuildStagesArr   mBuildStages;

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

	void setAtlasesPath(const string& path);

	string getAtlasesPath() const;
	string getBuildAssetsPath() const;
	string getAssetsPath() const;

	cImageAtlasInfo* createImageAtlas(const string& name, const vec2f& maxSize, cBuildPathInfo* attachingPath = NULL);
	void removeAtlas(const string& name);
	cImageAtlasInfo* getAtlas(const string& name);

private:
	void loadBuildInfo(bool errors = false);

	void updateBuildConfig();
	void gatherAssetsFileInfos(BuildFileInfoArr& filesMeta);
	void gatherAssetsFileInfosFromFolder(cPathInfo& pathInfo, BuildFileInfoArr& filesMeta);
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
