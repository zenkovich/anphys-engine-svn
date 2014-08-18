#ifndef BUILD_SYSTEM_H
#define BUILD_SYSTEM_H

#include "public.h"

#include "util/singleton.h"
#include "util/file_system/file.h"
#include "util/time_utils.h"

OPEN_O2_NAMESPACE

class cBuildConfig;

class cBuildSystem: public cSingleton<cBuildSystem>
{
public:
	struct FileMeta
	{
		string   mPath;
		bool     mBuildIncluded;
		uint32   mSize;
		WideTime mWritedTime;

		virtual cFileType::value getType() const { return cFileType::FT_FILE; }
	};

	struct ImageFileMeta: public FileMeta
	{
		string mAtlasName;
		virtual cFileType::value getType() const { return cFileType::FT_IMAGE; }
	};

	typedef vector<cBuildConfig*> BuildConfigsVec;
	typedef vector<FileMeta*> FilesMetaVec;

protected:
	string          mProjectName;
	string          mProjectPath;
	BuildConfigsVec mBuildConfigs;
	cBuildConfig*   mActiveBuildConfig;

public:
	cBuildSystem(const string& configFile);
	~cBuildSystem();

	void rebuildAssets(bool forcible = false);
};

CLOSE_O2_NAMESPACE

#endif //BUILD_SYSTEM_H
