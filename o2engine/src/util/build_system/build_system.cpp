#include "build_system.h"

#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(cBuildSystem::FileMeta)
{
	SERIALIZE_ID(mPath, "path");
	SERIALIZE_ID(mBuildIncluded, "buildIncluded");
	SERIALIZE_ID(mSize, "size");
	SERIALIZE_ID(mWritedTime, "writedTime");

	return true;
}

SERIALIZE_INHERITED_METHOD_IMPL(cBuildSystem::ImageFileMeta)
{
	SERIALIZE_ID(mAtlasName, "atlas");

	return true;
}

cBuildSystem::cBuildSystem(const string& configFile)
{
	pugi::xml_document doc;
	if (!cXmlTools::loadFromFile(configFile, doc))
	{
		logError("Failed to load build config file: %s", configFile.c_str());
		return;
	}


}

cBuildSystem::~cBuildSystem()
{

}

void cBuildSystem::rebuildAssets( bool forcible /*= false*/ )
{

}

CLOSE_O2_NAMESPACE