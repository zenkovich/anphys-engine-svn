#include "2d_render_state_base_interface.h"

#include "render/camera/camera_2d.h"

gr2DRenderStateBaseInterface::gr2DRenderStateBaseInterface():grRenderState(), mCamera(NULL), mPolygonCullMode(PCM_COUNETRCLOCKWISE)
{
}

gr2DRenderStateBaseInterface::gr2DRenderStateBaseInterface( grRenderBase* render ):grRenderState(render), mCamera(NULL), 
	mPolygonCullMode(PCM_COUNETRCLOCKWISE)
{
}

gr2DRenderStateBaseInterface::~gr2DRenderStateBaseInterface()
{
}
