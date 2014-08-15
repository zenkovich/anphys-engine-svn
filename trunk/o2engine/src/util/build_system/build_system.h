#ifndef BUILD_SYSTEM_H
#define BUILD_SYSTEM_H

#include "public.h"

#include "util/singleton.h"

OPEN_O2_NAMESPACE

class cBuildSystem: public cSingleton<cBuildSystem>
{
	string mBuildName;
	string mProjectName;
	string mProjectPath;

public:
	void rebuildAssets();
};

CLOSE_O2_NAMESPACE

#endif //BUILD_SYSTEM_H
