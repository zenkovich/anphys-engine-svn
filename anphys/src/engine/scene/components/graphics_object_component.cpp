#include "graphics_object_component.h"

//render
#include "render/render_objects/render_object.h"
#include "render/render_objects/object_manager.h"

cGraphicsObjectComponent::cGraphicsObjectComponent():cObjectComponent(), mRenderObject(NULL)
{
}

cGraphicsObjectComponent::cGraphicsObjectComponent(grRenderObject* renderObject):
	cObjectComponent(), mRenderObject(renderObject)
{
}

cGraphicsObjectComponent::cGraphicsObjectComponent(cObject* ownerObject):
	cObjectComponent(ownerObject), mRenderObject(NULL)
{
}

cGraphicsObjectComponent::cGraphicsObjectComponent(cObject* ownerObject, grRenderObject* renderObject):
	cObjectComponent(ownerObject), mRenderObject(renderObject)
{
}

cGraphicsObjectComponent::~cGraphicsObjectComponent()
{
	mRenderObject->mRenderObjectsManager->removeObject(mRenderObject);
	mRenderObject = NULL;
}
