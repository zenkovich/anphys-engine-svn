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
		string   mPath;
		bool     mBuildIncluded;
		uint32   mSize;
		WideTime mWritedTime;

		virtual cFileType::value getType() const { return cFileType::FT_FILE; }

		SERIALIZE_METHOD_DECL();
	};

	struct ImageFileMeta: public FileMeta
	{
		string mAtlasName;
		virtual cFileType::value getType() const { return cFileType::FT_IMAGE; }

		SERIALIZE_INHERITED_METHOD_DECL(FileMeta);
	};

	typedef vector<cBuildConfig*> BuildConfigsVec;
	typedef vector<FileMeta*> FilesMetaVec;

protected:
	string          mProjectName;
	string          mProjectPath;
	BuildConfigsVec mBuildConfigs;
	cBuildConfig*   mActiveBuildConfig;
	cBuildInfo*     mBuildInfo;

public:
	cBuildSystem(const string& projectPath);
	~cBuildSystem();

	void cleanUpBuildedAssest();
	void saveConfig();
	void rebuildAssets(bool forcible = false);

	string getBuildAssetsPath() const;

private:
	void updateBuildConfig();
	void updateBuildConfigPath(cPathInfo pathInfo);
	void copyNonBuildingFiles();
};

CLOSE_O2_NAMESPACE

#endif //BUILD_SYSTEM_H
