#include "render3d_object_component.h"

//render
#include "../../render/render_3d_object.h"
#include "../../render/object_manager.h"

cRender3DObjectComponent::cRender3DObjectComponent():cObjectComponent(), mRender3DObject(NULL)
{
}

cRender3DObjectComponent::cRender3DObjectComponent(grRender3DObject* renderObject):
	cObjectComponent(), mRender3DObject(renderObject)
{
}

cRender3DObjectComponent::cRender3DObjectComponent(cObject* ownerObject):
	cObjectComponent(ownerObject), mRender3DObject(NULL)
{
}

cRender3DObjectComponent::cRender3DObjectComponent(cObject* ownerObject, grRender3DObject* renderObject):
	cObjectComponent(ownerObject), mRender3DObject(renderObject)
{
}

cRender3DObjectComponent::~cRender3DObjectComponent()
{
	mRender3DObject->mRenderObjectsManager->removeObject(mRender3DObject);
	mRender3DObject = NULL;
}


