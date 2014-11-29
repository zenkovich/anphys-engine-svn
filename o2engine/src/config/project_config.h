#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include "public.h"
#include "app/application.h"
#include "util\serialize_util.h"

OPEN_O2_NAMESPACE

class ProjectBuildConfig;

class ProjectConfig: public cSerializable
{
	string              mProjectName;
	bool                mAssetsUsesMetaIds;
	ProjectBuildConfig* mBuildConfig;

public:
	ProjectConfig();
	~ProjectConfig();

	string getProjectName() const;	
	ProjectBuildConfig* getBuildConfig() const;

	SERIALIZBLE_METHODS(ProjectConfig);

protected:
	void initializeDefault(const string& configFilePath);
};

inline ProjectBuildConfig* projectBuildConfig() { return projectConfig()->getBuildConfig(); }

CLOSE_O2_NAMESPACE

#endif // PROJECT_CONFIG_H
