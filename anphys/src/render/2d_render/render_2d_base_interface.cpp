#include "render_2d_base_interface.h"

//render
#include "render/render.h"
#include "render/camera/camera_2d.h"

//framesystem
#include "engine/frame_system/render_frame.h"


grRender2DBaseInterface::grRender2DBaseInterface( grRenderBaseInterface* render )
{
	mRender = static_cast<grRender*>(render);
	mReady = mRendering = false;
}

grRender2DBaseInterface::~grRender2DBaseInterface()
{
	safe_release(mCamera);
}
