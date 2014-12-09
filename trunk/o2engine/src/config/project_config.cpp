#include "project_config.h"

#include "build_config.h"

OPEN_O2_NAMESPACE

ProjectConfig::ProjectConfig():
	mBuildConfig(NULL), mAssetsUsesMetaIds(true)
{
	string cfgFilePath = PROJECT_CONFIG_FILE_PATH;

	cSerializer serializer;
	if (!serializer.load(cfgFilePath))
	{
		logError("Failed to load Project Config. Path: %s. Initializing default values.", cfgFilePath.c_str());
		initializeDefault(cfgFilePath);
		return;
	}
	else serializer.serialize(this, "config");

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
	serializer.serialize(this, "config");
	serializer.save(configFilePath);
}

bool ProjectConfig::serialize(cSerializer* serializer)
{
	SERIALIZE_ID(mProjectName, "projectName");
	SERIALIZE_ID(mAssetsUsesMetaIds, "assetsUsingMetaIds");

	return true;
}


CLOSE_O2_NAMESPACE