#ifndef BUILD_CONFIG_H
#define BUILD_CONFIG_H

#include "public.h"
#include "atlas_info.h"
#include "build_info.h"

OPEN_O2_NAMESPACE

class cBuildConfig: public cBuildInfo
{
public:
	string mName;
	string mTargetPlatform;

	SERIALIZBLE_INHERITED_METHODS(cBuildConfig, cBuildInfo);
};

CLOSE_O2_NAMESPACE

#endif // BUILD_CONFIG_H
