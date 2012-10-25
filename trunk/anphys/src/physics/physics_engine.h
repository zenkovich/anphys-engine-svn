#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>

struct phScene;

struct phEngine
{
	typedef std::vector<phScene*> ScenesList;

	ScenesList mScenes;


	phEngine();
	~phEngine();

	phScene* addScene(phScene* scene);
	bool removeScene(phScene* scene);
	void removeAllScenes();

	void update(float dt);
};



#endif //PHYSICS_ENGINE_H