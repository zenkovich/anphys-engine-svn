#ifndef RENDER_STUFF_H
#define RENDER_STUFF_H

#include "../../engine/engine_incl.h"

struct cRenderStuff:public cSingleton<cRenderStuff>
{
	grRenderMulti3DObject* mRedArrows;
	grRenderMulti3DObject* mGreenArrows;
	grRenderMulti3DObject* mBlueArrows;
	grRenderMulti3DObject* mRedCubes;
	grRenderMulti3DObject* mGreenCubes;
	grRenderMulti3DObject* mBlueCubes;

	cRenderStuff() {}
	~cRenderStuff() {}

	void initialize(grRenderSceneBaseInterface* scene);
	
	gr3DObjectConditionContainer& addRedArrow(const vec3& start, const vec3& end);
	gr3DObjectConditionContainer& addGreenArrow(const vec3& start, const vec3& end);
	gr3DObjectConditionContainer& addBlueArrow(const vec3& start, const vec3& end);
	
	gr3DObjectConditionContainer& addRedCube(const vec3& pos, const vec3& size = vec3(0.1f));
	gr3DObjectConditionContainer& addGreenCube(const vec3& pos, const vec3& size = vec3(0.1f));
	gr3DObjectConditionContainer& addBlueCube(const vec3& pos, const vec3& size = vec3(0.1f));
};

#define getRenderStuff() cRenderStuff::instance()

#endif //RENDER_STUFF_H