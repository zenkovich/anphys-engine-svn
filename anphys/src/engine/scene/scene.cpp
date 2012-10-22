#include "scene.h"

//physics
#include "../../physics/physics_scene.h"

//render
#include "../../render/render_scene_base_interface.h"

//engine
#include "../engine.h"
#include "../render_system/render_frame.h"

cScene::cScene():mPhysicsScene(NULL), mRenderScene(NULL)
{
}

cScene::cScene(grRenderFrame* renderFrameOwner):mRenderFrameOwner(renderFrameOwner)
{
}

cScene::~cScene()
{
}

cObject* cScene::addObject(cObject* newObject)
{
	return NULL;
}

bool cScene::removeObject(cObject* object)
{
	return false;
}

void cScene::removeAllObjects()
{
}

cObject* cScene::getObject(int idx)
{
	return NULL;
}

cScene::ObjectsList& cScene::getObjectsList()
{
	return mObjects;
}

void cScene::draw()
{
}

void cScene::update(float dt)
{
}
