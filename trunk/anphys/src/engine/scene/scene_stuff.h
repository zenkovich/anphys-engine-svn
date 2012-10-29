#ifndef ENGINE_SCENE_STUFF_H
#define ENGINE_SCENE_STUFF_H

#include "../../util/utils.h"

struct cScene;
struct cObject;

struct cSceneStuff
{
	cScene* mScene;

	cSceneStuff(cScene* scene);


	cObject* createRigidWoodBox(const vec3& pos, const vec3& size, const mat3x3& orient = nullMatr() );
};

#endif //ENGINE_SCENE_STUFF_H