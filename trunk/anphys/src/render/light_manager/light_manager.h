#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <vector>
#include <string>

struct grLight;
struct grRender;
struct grRenderBaseInterface;
struct cLogStream;

struct grLightManager
{
	typedef std::vector<grLight*> LightsList;

	grRender*   mRender;
	LightsList  mLights;
	cLogStream* mLog;

//functions
	grLightManager(grRenderBaseInterface* render);
	~grLightManager();

	grLight* addLight(grLight* light);

	void activeLight(int idx, bool active);

	grLight* getLight(int idx, bool warnings = true);

	bool removeLight(grLight* light);
	bool removeLight(int idx);
	bool removeAllLights();
};

#endif //LIGHT_MANAGER_H