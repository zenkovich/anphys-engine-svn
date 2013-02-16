#include "render_frame.h"

//physics
#include "../../physics/physics_engine.h"


grRenderFrame::grRenderFrame()
{
	mPhysics = new phEngine;
}

grRenderFrame::~grRenderFrame()
{
	safe_release(mPhysics);
}
