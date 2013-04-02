#ifndef RENDER_BASE_INTERFACE_H
#define RENDER_BASE_INTERFACE_H

#include <string>
#include <vector>
#include "../util/math/mmath.h"

struct cLogStream;
struct grTextureManager;
struct grLightManager;
struct grMaterialManager;
struct grSurfaceMaterialManager;
struct grSceneManager;
struct grRenderTarget;
struct grBackbufferRenderTarget;

struct grRenderBaseInterface
{
	typedef std::vector<grRenderTarget*> RenderTargetsList;

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

	virtual void resize(const vec2& size);

	bool bindRenderTarget(grRenderTarget* renderTarget);
	bool unbindRenderTarget(grRenderTarget* renderTarget);
	grRenderTarget* getCurrentRenderTarget();

	virtual const char* getRenderName() { return "render base"; }
};

#endif //RENDER_BASE_INTERFACE_H