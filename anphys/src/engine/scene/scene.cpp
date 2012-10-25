#include "scene.h"

#include <algorithm>

//physics
#include "../../physics/physics_engine.h"
#include "../../physics/physics_scene.h"

//render
#include "../../render/render.h"
#include "../../render/scene_manager.h"
#include "../../render/render_scene_base_interface.h"

//engine
#include "../engine.h"
#include "../render_system/render_frame.h"
#include "object.h"

cScene::cScene():mPhysicsScene(NULL), mRenderScene(NULL), mReady(false)
{
}

cScene::cScene(grRenderFrame* renderFrameOwner):mRenderFrameOwner(renderFrameOwner), mReady(false)
{
	initialize();
}

cScene::~cScene()
{
	removeAllObjects();
}

cObject* cScene::addObject(cObject* newObject)
{
	if (!mReady) return NULL;

	mObjects.push_back(newObject);
	return newObject;
}

bool cScene::removeObject(cObject* object)
{
	if (!mReady) return false;

	ObjectsList::iterator fnd = std::find(mObjects.begin(), mObjects.end(), object);
	if (fnd == mObjects.end()) return false;

	safe_release(object);
	mObjects.erase(fnd);

	return true;
}

void cScene::removeAllObjects()
{
	if (!mReady) return;

	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
		safe_release(*it);

	mObjects.clear();
}

cObject* cScene::getObject(int idx)
{
	if (!mReady) return NULL;

	if (idx < 0 || idx >= (int)mObjects.size()) return NULL;

	return mObjects[idx];
}

cScene::ObjectsList& cScene::getObjectsList()
{
	return mObjects;
}

void cScene::draw()
{
	if (!mReady) return;

	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
	{
		(*it)->draw();
	}
}

void cScene::update(float dt)
{
	if (!mReady) return;

	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
	{
		(*it)->update(dt);
	}
}

void cScene::initialize()
{
	if (!mRenderFrameOwner) return;

	if (mReady)
	{
		mRenderFrameOwner->mPhysics->removeScene(mPhysicsScene);
		mRenderFrameOwner->mRender->mSceneManager->removeScene(mRenderScene);
	}

	mReady = false;

	mPhysicsScene = mRenderFrameOwner->mPhysics->addScene(new phScene());

	mRenderScene = mRenderFrameOwner->mRender->mSceneManager->addScene(new grRenderSceneBaseInterface());

	mReady = true;
}
