#include "light_manager.h"

#include <algorithm>

#include "render/render.h"
#include "light.h"
#include "util/memory/mem_utils.h"

grLightManager::grLightManager(grRenderBaseInterface* render)
{
	mRender = static_cast<grRender*>(render);
}

grLightManager::~grLightManager()
{
	removeAllLights();
}

grLight* grLightManager::addLight(grLight* light)
{
	mLights.push_back(light);
	light->mLightManager = this;
	light->idx(mLights.size() - 1);
	*mRender->mRenderLog << formatStr("added light %x\n", light);
	return light;
}

void grLightManager::activeLight(int idx, bool active)
{
	mLights[idx]->setLightActive(active);
}

grLight* grLightManager::getLight(int idx)
{
	if (idx < 0 || idx >= (int)mLights.size()) return NULL;
	return mLights[idx];
}

bool grLightManager::removeLight(grLight* light)
{
	LightsList::iterator it = std::find(mLights.begin(), mLights.end(), light);
	if (it == mLights.end()) return false;

	safe_release(*it);
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

	return true;
}
