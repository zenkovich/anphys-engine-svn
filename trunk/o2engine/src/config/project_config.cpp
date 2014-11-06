#include "project_config.h"

#include "build_config.h"
#include "util\serialize_util.h"

OPEN_O2_NAMESPACE

ProjectConfig::ProjectConfig():
	mBuildConfig(NULL)
{
	string cfgFilePath = "../../../config.xml";

	cSerializer serializer;
	if (!serializer.load(cfgFilePath))
	{
		logError("Failed to load Project Config. Path: %s. Initializing default values.", cfgFilePath.c_str());
		initializeDefault(cfgFilePath);
		return;
	}
	else 
	{
		serializer.serialize(mProjectName, "projectName");
	}

	mBuildConfig = mnew ProjectBuildConfig(this);
}

ProjectConfig::~ProjectConfig()
{
	safe_release(mBuildConfig);
}

string ProjectConfig::getProjectName() const
{
	return mProjectName;
}

ProjectBuildConfig* ProjectConfig::getBuildConfig() const
{
	return mBuildConfig;
}

void ProjectConfig::initializeDefault(const string& configFilePath)
{
	mProjectName = "unnamed";

	cSerializer serializer;
	serializer.serialize(mProjectName, "projectName");
	serializer.save(configFilePath);
}


CLOSE_O2_NAMESPACE