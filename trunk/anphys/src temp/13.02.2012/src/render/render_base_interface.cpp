#include "render_base_interface.h"

#include "cameras_manager.h"
#include "texture_manager.h"
#include "light_manager.h"
#include "material_manager.h"
#include "surface_material_manager.h"
#include "scene_manager.h"

#include "camera.h"

#include "../util/memory/mem_utils.h"


grRenderBaseInterface::grRenderBaseInterface()
{
	mCameras = new grCamerasManager(this);
	mTextures = new grTextureManager(this);
	mLights = new grLightManager(this);
	mMaterials = new grMaterialManager(this);
	mSurfaceMaterials = new grSurfaceMaterialManager(this);
	mSceneManager = new grSceneManager(this);
}

grRenderBaseInterface::~grRenderBaseInterface()
{
	safe_release(mCameras);
	safe_release(mTextures);
	safe_release(mLights);
	safe_release(mMaterials);
	safe_release(mSurfaceMaterials);
	safe_release(mSceneManager);
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
}

void grRenderBaseInterface::postRender()
{
}

