#ifndef BUILD_CONFIG_H
#define BUILD_CONFIG_H

#include "public.h"

OPEN_O2_NAMESPACE

class ProjectConfig;
class asAssetConfig;
	
class ProjectBuildConfig
{
public:
	typedef array<asAssetConfig*> AssetsConfigsArr;

protected:
	string           mName;
	ProjectConfig*   mProjectConfig;

public:
	ProjectBuildConfig(ProjectConfig* projConfig);
	~ProjectBuildConfig();

	string getName() const;
	ProjectConfig* getProjectConfigs() const;

protected:
	void initializeDefault(const string& configFilePath);
};

CLOSE_O2_NAMESPACE

#endif // BUILD_CONFIG_H
