#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include "public.h"
#include "app/application.h"

OPEN_O2_NAMESPACE

class ProjectBuildConfig;

class ProjectConfig
{
	string              mProjectName;
	ProjectBuildConfig* mBuildConfig;

public:
	ProjectConfig();
	~ProjectConfig();

	string getProjectName() const;	
	ProjectBuildConfig* getBuildConfig() const;

protected:
	void initializeDefault(const string& configFilePath);
};

inline ProjectBuildConfig* projectBuildConfig() { return projectConfig()->getBuildConfig(); }

CLOSE_O2_NAMESPACE

#endif // PROJECT_CONFIG_H
