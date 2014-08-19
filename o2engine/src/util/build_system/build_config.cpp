#include "build_config.h"

OPEN_O2_NAMESPACE

SERIALIZE_INHERITED_METHOD_IMPL(cBuildConfig)
{
	SERIALIZE_ID(mName, "name");
	SERIALIZE_ID(mTargetPlatform, "platform");

	return true;
}

CLOSE_O2_NAMESPACE