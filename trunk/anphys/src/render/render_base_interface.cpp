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


grRenderBaseInterface::grRenderBaseInterface()
{
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
}

void grRenderBaseInterface::update(float dt)
{
	mCameras->mActiveCamera->update(dt);

	mSceneManager->update(dt);
}

void grRenderBaseInterface::preRender()
{
	
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
}

