#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <vector>
#include <string>

struct grLight;
struct grRender;
struct grRenderBaseInterface;

struct grLightManager
{
	grRender* mRender;

	typedef std::vector<grLight*> LightsList;
	LightsList mLights;

	grLightManager(grRenderBaseInterface* render);
	~grLightManager();

	grLight* addLight(grLight* light);

	void activeLight(int idx, bool active);

	grLight* getLight(int idx);

	bool removeLight(grLight* light);
	bool removeLight(int idx);
	bool removeAllLights();
};

#endif //LIGHT_MANAGER_H