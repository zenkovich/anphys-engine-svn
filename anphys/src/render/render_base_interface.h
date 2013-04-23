#ifndef RENDER_BASE_INTERFACE_H
#define RENDER_BASE_INTERFACE_H

#include <string>
#include <vector>
#include "util/math/mmath.h"

struct cLogStream;
struct grTextureManager;
struct grLightManager;
struct grMaterialManager;
struct grSurfaceMaterialManager;
struct grSceneManager;
struct grRenderTarget;
struct grBackbufferRenderTarget;
struct grRenderState;
struct grStencilBufferRenderTarget;

struct grRenderBaseInterface
{
	typedef std::vector<grRenderTarget*>              RenderTargetsList;
	typedef std::vector<grStencilBufferRenderTarget*> StencilBuffersList;

	grTextureManager*            mTextures;
	grLightManager*              mLights;
	grMaterialManager*           mMaterials;
	grSurfaceMaterialManager*    mSurfaceMaterials;
	grSceneManager*              mSceneManager;
							     
	RenderTargetsList            mRenderTargetsStack;
	grBackbufferRenderTarget*    mBackbufferRenderTarget;
							     
	grRenderState*               mCurrentRenderState;

	StencilBuffersList           mStencilBuffersStack;
	grStencilBufferRenderTarget* mCurrentStencilBuffer;

	cLogStream*                  mLog;

//functions
	grRenderBaseInterface();
	virtual ~grRenderBaseInterface();

	virtual void update(float dt);

	virtual void beginRender();
	virtual void render();
	virtual void endRender();

	virtual void swapFullscreen();

	virtual void resize(const vec2& size);

	bool bindRenderTarget(grRenderTarget* renderTarget);
	bool unbindRenderTarget(grRenderTarget* renderTarget);
	grRenderTarget* getCurrentRenderTarget();

	void bindRenderState(grRenderState* renderState);
	void unbindRenderState();
	grRenderState* getCurrentRenderState();

	virtual void bindStencilBuffer(grStencilBufferRenderTarget* stencilBuffer) {}
	virtual void unbindStencilBuffer(grStencilBufferRenderTarget* stencilBuffer = NULL) {}

	virtual const char* getRenderName() { return "render base"; }
};

#endif //RENDER_BASE_INTERFACE_H