#include "simple_3d_render_state_base_interface.h"

#include "render/camera/camera_3d.h"

grSimple3DRenderStateBaseInterface::grSimple3DRenderStateBaseInterface():
	grRenderState(), mAmbientColor(0.0f, 0.0f, 0.0f, 0.0f), mPolygonCullMode(PCM_NONE), mCamera(NULL)
{

}

grSimple3DRenderStateBaseInterface::grSimple3DRenderStateBaseInterface( grRenderBase* render ):
	grRenderState(render), mAmbientColor(0.0f, 0.0f, 0.0f, 0.0f), mPolygonCullMode(PCM_NONE), mCamera(NULL)
{

}

grSimple3DRenderStateBaseInterface::~grSimple3DRenderStateBaseInterface()
{
	safe_release(mCamera)
}
