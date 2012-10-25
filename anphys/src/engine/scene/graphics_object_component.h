#ifndef ENGINE_GRAPHICS_OBJECT_COMPONENT_H
#define ENGINE_GRAPHICS_OBJECT_COMPONENT_H

#include "object_component.h"

struct grRenderObject;

struct cGraphicsObjectComponent:public cObjectComponent
{
	grRenderObject* mRenderObject;


	cGraphicsObjectComponent();
	cGraphicsObjectComponent(grRenderObject* renderObject);
	cGraphicsObjectComponent(cObject* ownerObject);
	cGraphicsObjectComponent(cObject* ownerObject, grRenderObject* renderObject);
	virtual ~cGraphicsObjectComponent();

	ObjectComponentType::types getType() { return ObjectComponentType::GraphicsObject; }
};

#endif //ENGINE_GRAPHICS_OBJECT_COMPONENT_H