#include "build_config.h"

#include "util\serialize_util.h"
#include "asset_config.h"

OPEN_O2_NAMESPACE

ProjectBuildConfig::ProjectBuildConfig(ProjectConfig* projConfig):
	mProjectConfig(projConfig)
{
	string cfgFilePath = BUILD_CONFIG_FILE_PATH;

	cSerializer serializer;
	if (!serializer.load(cfgFilePath)) 
	{
		logError("Failed to load build config: %s. Initializing default.", cfgFilePath.c_str());
		initializeDefault(cfgFilePath);
		return;
	}

	serializer.serialize(mName, "buildName");
}

ProjectBuildConfig::~ProjectBuildConfig()
{
}

string ProjectBuildConfig::getName() const
{
	return mName;
}

ProjectConfig* ProjectBuildConfig::getProjectConfigs() const
{
	return mProjectConfig;
}

void ProjectBuildConfig::initializeDefault(const string& configFilePath)
{
	mName = "unnamed";

	cSerializer serializer(cSerializer::ST_SERIALIZE);
	serializer.serialize(mName, "buildName");
	serializer.save(configFilePath);
}

CLOSE_O2_NAMESPACE