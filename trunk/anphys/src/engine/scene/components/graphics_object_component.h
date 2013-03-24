#ifndef ENGINE_GRAPHICS_OBJECT_COMPONENT_H
#define ENGINE_GRAPHICS_OBJECT_COMPONENT_H

#include "../object_component.h"

struct grRenderObject;

struct cGraphicsObjectComponent:public cObjectComponent
{
	DEFINE_TYPE(cGraphicsObjectComponent)

	grRenderObject* mRenderObject;

//functions
	cGraphicsObjectComponent();
	cGraphicsObjectComponent(grRenderObject* renderObject);
	cGraphicsObjectComponent(cObject* ownerObject);
	cGraphicsObjectComponent(cObject* ownerObject, grRenderObject* renderObject);
	virtual ~cGraphicsObjectComponent();
};

#endif //ENGINE_GRAPHICS_OBJECT_COMPONENT_H