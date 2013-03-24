#include "light_manager.h"

#include <algorithm>

#include "render/render.h"
#include "light.h"
#include "util/memory/mem_utils.h"
#include "util/log/log_stream_in_file.h"
#include "util/log/log_system.h"

grLightManager::grLightManager(grRenderBaseInterface* render)
{
	mRender = static_cast<grRender*>(render);
	mLog = gLogSystem->addStream(new cLogStreamInFile("lights.txt"), "LightManager");
	mLog->mLogLevel = STD_LOG_LEVEL;
}

grLightManager::~grLightManager()
{
	removeAllLights();
	gLogSystem->removeStream(mLog);
}

grLight* grLightManager::addLight(grLight* light)
{
	mLights.push_back(light);
	light->mLightManager = this;
	light->idx(mLights.size() - 1);

	mLog->fout(1, "Added light %x", light);

	return light;
}

void grLightManager::activeLight(int idx, bool active)
{

	grLight* light = getLight(idx, false);
	if (!light) mLog->fout(1, "WARNING: Can't %s %i light (%i)", (active) ? "activate":"deactivate", idx, mLights.size());
	else        light->setLightActive(active);
}

grLight* grLightManager::getLight( int idx, bool warnings /*= true*/ )
{
	if (idx < 0 || idx >= (int)mLights.size()) 
	{
		if (warnings) mLog->fout(1, "WARNING: Can't get %i light (%i)", idx, mLights.size());
		return NULL;
	}

	return mLights[idx];
}

bool grLightManager::removeLight(grLight* light)
{
	LightsList::iterator it = std::find(mLights.begin(), mLights.end(), light);

	mLog->fout(1, "Removing light %x", light);

	safe_release(*it);
	if (it == mLights.end()) return false;
	mLights.erase(it);

	return true;
}

bool grLightManager::removeLight(int idx)
{
	return removeLight(getLight(idx));
}

bool grLightManager::removeAllLights()
{
	if (mLights.size() == 0) return false;
	for (LightsList::iterator it = mLights.begin(); it != mLights.end(); it++)
		safe_release(*it);

	mLights.clear();

	mLog->fout(1, "Removed all lights");

	return true;
}
