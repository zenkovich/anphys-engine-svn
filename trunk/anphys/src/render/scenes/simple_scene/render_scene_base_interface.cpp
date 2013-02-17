#include "render_scene_base_interface.h"

#include <algorithm>

#include "render/render_objects/render_object.h"
#include "render/render_objects/object_manager.h"
#include "render/render.h"
#include "render/scenes/scene_manager.h"


grRenderSceneBaseInterface::grRenderSceneBaseInterface(grSceneManager* sceneManager):mSceneManager(sceneManager)
{
	grRender* render = NULL;
	if (sceneManager) render = sceneManager->mRender;
	mObjects = new grRenderObjectsManager(render);
}

grRenderSceneBaseInterface::~grRenderSceneBaseInterface()
{
	mObjects->removeAllObjects();
	safe_release(mObjects);
}

void grRenderSceneBaseInterface::setSceneManager(grSceneManager* sceneManager)
{
	grRender* render = NULL;
	if (sceneManager) render = sceneManager->mRender;
	mObjects->mRender = render;
	mSceneManager = sceneManager;
}

void grRenderSceneBaseInterface::update(float dt)
{
	mObjects->update(dt);
}

void grRenderSceneBaseInterface::render()
{
	mObjects->render();
}
