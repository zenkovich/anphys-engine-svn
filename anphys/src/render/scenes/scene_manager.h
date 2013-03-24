#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "util/utils.h"
#include <vector>

struct grRender;
struct grRenderBaseInterface;
struct grRenderSceneBaseInterface;
struct cLogStream;

struct grSceneManager
{
	typedef std::vector<grRenderSceneBaseInterface*> ScenesList;

	grRender*   mRender;
	ScenesList  mScenes;
	cLogStream* mLog;

//functions
	grSceneManager(grRenderBaseInterface* render);
	~grSceneManager();

	grRenderSceneBaseInterface* addScene(grRenderSceneBaseInterface* newScene);
	bool removeScene(grRenderSceneBaseInterface* scene);

	void removeAllScenes();

	void update(float dt);
	void render();
};

#endif //SCENE_MANAGER_H