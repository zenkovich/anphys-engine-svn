#ifndef RENDER_BASE_INTERFACE_H
#define RENDER_BASE_INTERFACE_H

#include <string>
#include <vector>
#include "../util/math/mmath.h"

struct grCamerasManager;
struct cLogStream;
struct grTextureManager;
struct grLightManager;
struct grMaterialManager;
struct grSurfaceMaterialManager;
struct grSceneManager;
struct grRender2D;
struct grRenderTarget;
struct grBackbufferRenderTarget;

struct grRenderBaseInterface
{
	typedef std::vector<grRenderTarget*> RenderTargetsList;

	grRender2D*               mRender2D;
	grCamerasManager*         mCameras;
	grTextureManager*         mTextures;
	grLightManager*           mLights;
	grMaterialManager*        mMaterials;
	grSurfaceMaterialManager* mSurfaceMaterials;
	grSceneManager*           mSceneManager;

	RenderTargetsList         mRenderTargetsStack;
	grBackbufferRenderTarget* mBackbufferRenderTarget;

	cLogStream*               mLog;

//functions
	grRenderBaseInterface();
	virtual ~grRenderBaseInterface();

	virtual void update(float dt);
	virtual void preRender();
	virtual void render();
	virtual void postRender();

	virtual void swapFullscreen();

	bool setupRenderTarget(grRenderTarget* renderTarget);
	bool completeRenderTarget(grRenderTarget* renderTarget);

	virtual const char* getRenderName() { return "render base"; }
};

#endif //RENDER_BASE_INTERFACE_H