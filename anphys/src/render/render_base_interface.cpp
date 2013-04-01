#include "render_base_interface.h"

#include "camera/cameras_manager.h"
#include "camera/camera.h"
#include "texture_manager/texture_manager.h"
#include "light_manager/light_manager.h"
#include "material_manager/material_manager.h"
#include "surface/surface_material_manager.h"
#include "scenes/scene_manager.h"
#include "util/memory/mem_utils.h"
#include "2d_render/render_2d.h"
#include "render_target/render_target_interface.h"
#include "render_target/backbuffer_render_target.h"


grRenderBaseInterface::grRenderBaseInterface()
{
	mLog = gLogSystem->addStream(new cLogStreamInFile("render log.txt"), "RenderLog");

	mCameras = new grCamerasManager(this);
	mTextures = new grTextureManager(this);
	mLights = new grLightManager(this);
	mMaterials = new grMaterialManager(this);
	mSurfaceMaterials = new grSurfaceMaterialManager(this);
	mSceneManager = new grSceneManager(this);
	mRender2D = new grRender2D(this);
}

grRenderBaseInterface::~grRenderBaseInterface()
{
	safe_release(mCameras);
	safe_release(mTextures);
	safe_release(mLights);
	safe_release(mMaterials);
	safe_release(mSurfaceMaterials);
	safe_release(mSceneManager);
	safe_release(mRender2D);

	gLogSystem->removeStream(mLog);

	safe_release(mBackbufferRenderTarget);
}

void grRenderBaseInterface::update(float dt)
{
	mCameras->mActiveCamera->update(dt);

	mSceneManager->update(dt);
}

void grRenderBaseInterface::preRender()
{
	bindRenderTarget(mBackbufferRenderTarget);
}

void grRenderBaseInterface::render()
{
	mSceneManager->render();

	mRender2D->beginRender();
}

void grRenderBaseInterface::postRender()
{
	mRender2D->endRender();

	mTextures->processStreaming();

	unbindRenderTarget(mBackbufferRenderTarget);
}

void grRenderBaseInterface::swapFullscreen()
{
	mLog->fout(0, "WARNING: swapFullscreen() in %s not implemented", getRenderName());
}

bool grRenderBaseInterface::bindRenderTarget( grRenderTarget* renderTarget )
{
	mRenderTargetsStack.push_back(renderTarget);

	return renderTarget->begin();
}

bool grRenderBaseInterface::unbindRenderTarget( grRenderTarget* renderTarget )
{
	bool res = true;
	for (RenderTargetsList::reverse_iterator it = mRenderTargetsStack.rbegin(); it != mRenderTargetsStack.rend(); )
	{
		res = res && (*it)->finish();
		grRenderTarget* curr = *it;

		mRenderTargetsStack.erase(--it.base());

		if (curr == renderTarget) break;
	}

	if (mRenderTargetsStack.size() > 0) res = res && mRenderTargetsStack[mRenderTargetsStack.size() - 1]->begin();

	return res;
}

grRenderTarget* grRenderBaseInterface::getCurrentRenderTarget()
{
	if (mRenderTargetsStack.size() < 1) return NULL;

	return mRenderTargetsStack[mRenderTargetsStack.size() - 1];
}

void grRenderBaseInterface::resize( const vec2& size )
{
	mBackbufferRenderTarget->mSize = size;
}

