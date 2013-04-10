#include "render_base_interface.h"

#include "light_manager/light_manager.h"
#include "material_manager/material_manager.h"
#include "render_state/render_state_interface.h"
#include "render_target/backbuffer_render_target.h"
#include "render_target/render_target_interface.h"
#include "scenes/scene_manager.h"
#include "surface/surface_material_manager.h"
#include "texture_manager/texture_manager.h"
#include "util/memory/mem_utils.h"


grRenderBaseInterface::grRenderBaseInterface()
{
	mLog = gLogSystem->addStream(new cLogStreamInFile("render log.txt"), "RenderLog");

	mTextures = new grTextureManager(this);
	mLights = new grLightManager(this);
	mMaterials = new grMaterialManager(this);
	mSurfaceMaterials = new grSurfaceMaterialManager(this);
	mSceneManager = new grSceneManager(this);

	mCurrentRenderState = NULL;
}

grRenderBaseInterface::~grRenderBaseInterface()
{
	safe_release(mTextures);
	safe_release(mLights);
	safe_release(mMaterials);
	safe_release(mSurfaceMaterials);
	safe_release(mSceneManager);

	gLogSystem->removeStream(mLog);

	safe_release(mBackbufferRenderTarget);
}

void grRenderBaseInterface::update(float dt)
{
	mSceneManager->update(dt);
}

void grRenderBaseInterface::beginRender()
{
	bindRenderTarget(mBackbufferRenderTarget);
}

void grRenderBaseInterface::render()
{
	mSceneManager->render();
}

void grRenderBaseInterface::endRender()
{
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

	if (!renderTarget->begin()) return false;

	if (mCurrentRenderState)
		mCurrentRenderState->updateTransformations();

	return true;
}

bool grRenderBaseInterface::unbindRenderTarget( grRenderTarget* renderTarget )
{
	bool res = true;
	for (RenderTargetsList::reverse_iterator it = mRenderTargetsStack.rbegin(); it != mRenderTargetsStack.rend(); )
	{
		grRenderTarget* currentRenderTarget = *it;

		if (mCurrentRenderState)
			mCurrentRenderState->flush();

		res = res && currentRenderTarget->finish();

		mRenderTargetsStack.erase(--it.base());

		if (currentRenderTarget == renderTarget) break;
	}

	if (mRenderTargetsStack.size() > 0)
	{
		res = res && mRenderTargetsStack[mRenderTargetsStack.size() - 1]->begin();

		if (mCurrentRenderState)
			mCurrentRenderState->updateTransformations();
	}

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

void grRenderBaseInterface::bindRenderState( grRenderState* renderState )
{
	mCurrentRenderState = renderState;

	mCurrentRenderState->begin();
}

void grRenderBaseInterface::unbindRenderState()
{
	if (mCurrentRenderState)
		mCurrentRenderState->finish();

	mCurrentRenderState = NULL;
}

grRenderState* grRenderBaseInterface::getCurrentRenderState()
{
	return mCurrentRenderState;
}

