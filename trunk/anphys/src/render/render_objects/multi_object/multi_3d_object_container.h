#ifndef MULTI_3D_OBJECT_CONTAINER_H
#define MULTI_3D_OBJECT_CONTAINER_H

#include "multi_object_container.h"
#include "util/utils.h"
#include "render/render_objects/3d/render_3d_object.h"

struct gr3DObjectConditionContainer:public grObjectConditionContainer
{
	
	mat3x3 mOrient;
	vec3 mPosition;
	grRender3DObject* mObject;

//functions
	gr3DObjectConditionContainer(mat3x3 orient, vec3 position):mOrient(orient), mPosition(position) { mDraw = true; }
	
	void setObject(grRenderObject* obj) { mObject = static_cast<grRender3DObject*>(obj); }

	void update(float dt) {}
	void render() 
	{ 
		if (!mDraw) return; 
		mObject->mOrient = mOrient;
		mObject->mPosition = mPosition; 
		mObject->update(0); 
		mObject->render(); 
	}
};

inline gr3DObjectConditionContainer* grMultiVector(vec3 start, vec3 end) 
{ 
	return new gr3DObjectConditionContainer(scaledVectorOrient(end - start, 0.1f, 0.1f), start); 
}

#endif //MULTI_3D_OBJECT_CONTAINER_H