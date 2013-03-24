#ifndef RENDER_BASE_INTERFACE_H
#define RENDER_BASE_INTERFACE_H

#include "../util/math/mmath.h"
#include <string>

struct grCamerasManager;
struct cLogStream;
struct grTextureManager;
struct grLightManager;
struct grMaterialManager;
struct grSurfaceMaterialManager;
struct grSceneManager;
struct grRender2D;

struct grRenderBaseInterface
{
	grRender2D*               mRender2D;
	grCamerasManager*         mCameras;
	grTextureManager*         mTextures;
	grLightManager*           mLights;
	grMaterialManager*        mMaterials;
	grSurfaceMaterialManager* mSurfaceMaterials;
	grSceneManager*           mSceneManager;

	cLogStream*               mLog;

//functions
	grRenderBaseInterface();
	virtual ~grRenderBaseInterface();

	virtual void update(float dt);
	virtual void preRender();
	virtual void render();
	virtual void postRender();

	virtual void swapFullscreen();

	virtual const char* getRenderName() { return "render base"; }
};

#endif //RENDER_BASE_INTERFACE_H