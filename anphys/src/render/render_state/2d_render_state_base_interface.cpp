#include "2d_render_state_base_interface.h"

#include "render/camera/camera_2d.h"

gr2DRenderStateBaseInterface::gr2DRenderStateBaseInterface():grRenderState(), mCamera(NULL)
{
}

gr2DRenderStateBaseInterface::gr2DRenderStateBaseInterface( grRenderBase* render ):grRenderState(render), mCamera(NULL)
{
}

gr2DRenderStateBaseInterface::~gr2DRenderStateBaseInterface()
{
	safe_release(mCamera);
}
