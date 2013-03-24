#ifndef RENDER_3D_OBJECT_BASE_INTERFACE_H
#define RENDER_3D_OBJECT_BASE_INTERFACE_H

#include "render/render_objects/render_object.h"
#include "render/render_objects/object_manager.h"
#include "util/other/type_intexation.h"

struct grRender3DObjectBaseInterface:public grRenderObject
{
	vec3   mPosition;
	mat3x3 mOrient;
	mat4x4 mTransform;

//functions
	grRender3DObjectBaseInterface():grRenderObject() {}
	grRender3DObjectBaseInterface(grRenderObjectsManager* objectsManager):grRenderObject(objectsManager) {}
	
	virtual void setObjectManager(grRenderObjectsManager* objectsManager) {}

	virtual void render() { }
	virtual void update(float dt) { mTransform = m3to4(mOrient, mPosition); }
};

#endif //RENDER_3D_OBJECT_BASE_INTERFACE_H