#include "simple_3d_render_state_base_interface.h"


grSimple3DRenderStateBaseInterface::grSimple3DRenderStateBaseInterface():
	grRenderState(), mAmbientColor(0.0f, 0.0f, 0.0f, 0.0f), mPolygonCullMode(PCM_NONE)
{

}

grSimple3DRenderStateBaseInterface::grSimple3DRenderStateBaseInterface( grRenderBase* render ):
	grRenderState(render), mAmbientColor(0.0f, 0.0f, 0.0f, 0.0f), mPolygonCullMode(PCM_NONE)
{

}

grSimple3DRenderStateBaseInterface::~grSimple3DRenderStateBaseInterface()
{

}
