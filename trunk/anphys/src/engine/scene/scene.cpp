#include "scene.h"

#include <algorithm>

//physics
#include "physics/physics_engine.h"
#include "physics/scenes/physics_scene.h"

//render
#include "render/render.h"
#include "render/scenes/scene_manager.h"
#include "render/scenes/simple_scene/render_scene_base_interface.h"

//engine
#include "engine/engine.h"
#include "engine/frame_system/render_frame.h"
#include "engine/scene/object.h"
#include "engine/scene/scene_stuff.h"

cScene::cScene():mPhysicsScene(NULL), mRenderScene(NULL), mReady(false), mLog(NULL)
{
}

cScene::cScene(grRenderFrame* renderFrameOwner, const std::string& sceneId):mRenderFrameOwner(renderFrameOwner), 
	mReady(false), mSceneId(sceneId), mLog(NULL)
{
	initialize();
}

cScene::~cScene()
{
	removeAllObjects();

	safe_release(mSceneStuff);

	gLogSystem->removeStream(mLog);
}

cObject* cScene::addObject(cObject* newObject)
{
	if (!mReady) return NULL;

	mLog->fout(1, "Add object: %x", newObject);

	mObjects.push_back(newObject);
	return newObject;
}

bool cScene::removeObject(cObject* object)
{
	if (!mReady) return false;

	ObjectsList::iterator fnd = std::find(mObjects.begin(), mObjects.end(), object);
	if (fnd == mObjects.end()) return false;

	mLog->fout(1, "Removing object %x", object);

	safe_release(object);
	mObjects.erase(fnd);

	return true;
}

void cScene::removeAllObjects()
{
	if (!mReady) return;

	mLog->fout(1, "Remove all objects: %i", mObjects.size());

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

	safe_release(mLog);
	mLog = gLogSystem->addStream(new cLogStreamInFile("scene_" + mSceneId + "_log.txt"), "Scene" + mSceneId + "Log");
	mLog->mLogLevel = INITIAL_SCENES_LOG_LEVEL;

	mLog->fout(1, "initializing scene: ready = %s", (mReady) ? "true":"false");

	if (mReady)
	{
		mLog->fout(2, "Removing Scenes: render scene %x, physics scene %x", mRenderScene, mPhysicsScene);
		mRenderFrameOwner->mPhysics->removeScene(mPhysicsScene);
		mRenderFrameOwner->mRender->mSceneManager->removeScene(mRenderScene);
	}

	mReady = false;

	mLog->fout(2, "Creating physics scene...");
	mPhysicsScene = mRenderFrameOwner->mPhysics->addScene(new phScene());
	mLog->fout(2, "Good. %x", mPhysicsScene);

	mLog->fout(2, "Creating render scene...");
	mRenderScene = mRenderFrameOwner->mRender->mSceneManager->addScene(new grRenderSceneBaseInterface());
	mLog->fout(2, "Good. %x", mRenderScene);
	
	mLog->fout(2, "Creating SceneStuff...");
	mSceneStuff = new cSceneStuff(this);
	mLog->fout(2, "Good. %x", mSceneStuff);

	mReady = true;
}
