#ifndef ENGINE_RENDER3D_OBJECT_COMPONENT_H
#define ENGINE_RENDER3D_OBJECT_COMPONENT_H

#include "engine/scene/object_component.h"

struct grRender3DObject;

struct cRender3DObjectComponent:public cObjectComponent
{
	DEFINE_TYPE(cRender3DObjectComponent)

	grRender3DObject* mRender3DObject;

//functions
	cRender3DObjectComponent();
	cRender3DObjectComponent(grRender3DObject* renderObject);
	cRender3DObjectComponent(cObject* ownerObject);
	cRender3DObjectComponent(cObject* ownerObject, grRender3DObject* renderObject);
	virtual ~cRender3DObjectComponent();
};

#endif //ENGINE_RENDER3D_OBJECT_COMPONENT_H