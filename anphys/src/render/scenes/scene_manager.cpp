#include "scene_manager.h"

#include <algorithm>

#include "render/render.h"
#include "render/scenes/simple_scene/render_scene_base_interface.h"


grSceneManager::grSceneManager(grRenderBaseInterface* render)
{
	mRender = static_cast<grRender*>(render);
	mLog = gLogSystem->addStream(new cLogStreamInFile("scenes_manager.txt"), "ScenesManager");
	mLog->mLogLevel = STD_LOG_LEVEL;
}

grSceneManager::~grSceneManager()
{
	removeAllScenes();
	gLogSystem->removeStream(mLog);
}

grRenderSceneBaseInterface* grSceneManager::addScene(grRenderSceneBaseInterface* newScene)
{
	mScenes.push_back(newScene);
	newScene->setSceneManager(this);

	mLog->fout(1, "Added scene %x", newScene);

	return newScene;
}

bool grSceneManager::removeScene(grRenderSceneBaseInterface* scene)
{
	ScenesList::iterator it = std::find(mScenes.begin(), mScenes.end(), scene);

	if (it == mScenes.end()) return false;

	mLog->fout(1, "Removing scene %x", *it);

	safe_release(scene);
	mScenes.erase(it);
	
	return true;
}

void grSceneManager::removeAllScenes()
{
	for (ScenesList::iterator it = mScenes.begin(); it != mScenes.end(); it++)
		safe_release(*it);

	mScenes.clear();

	mLog->fout(1, "Removed all scenes");
}

void grSceneManager::update(float dt)
{
	for (ScenesList::iterator it = mScenes.begin(); it != mScenes.end(); it++)
		(*it)->update(dt);
}

void grSceneManager::render()
{
	for (ScenesList::iterator it = mScenes.begin(); it != mScenes.end(); it++)
		(*it)->render();
}

