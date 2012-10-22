#include "render_scene_base_interface.h"

#include "render_object.h"
#include "object_manager.h"
#include "render.h"
#include "scene_manager.h"

#include <algorithm>

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
}

void grRenderSceneBaseInterface::update(float dt)
{
	mObjects->update(dt);
}

void grRenderSceneBaseInterface::render()
{
	mObjects->render();
}
