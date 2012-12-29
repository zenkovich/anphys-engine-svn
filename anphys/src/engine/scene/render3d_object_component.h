#ifndef ENGINE_RENDER3D_OBJECT_COMPONENT_H
#define ENGINE_RENDER3D_OBJECT_COMPONENT_H

#include "object_component.h"

struct grRender3DObject;

struct cRender3DObjectComponent:public cObjectComponent
{
	grRender3DObject* mRender3DObject;


	cRender3DObjectComponent();
	cRender3DObjectComponent(grRender3DObject* renderObject);
	cRender3DObjectComponent(cObject* ownerObject);
	cRender3DObjectComponent(cObject* ownerObject, grRender3DObject* renderObject);
	virtual ~cRender3DObjectComponent();

	ObjectComponentType::types getType() { return ObjectComponentType::Render3DObject; }
};

#endif //ENGINE_RENDER3D_OBJECT_COMPONENT_H