#ifndef 2D_RENDER_STATE_H
#define 2D_RENDER_STATE_H

#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/render_state/2d_render_state_d3d8.h"
#endif // RENDER_D3D8

struct gr2DRenderState:public gr2DRenderStateBase
{
	gr2DRenderState():gr2DRenderStateBase() {}
	gr2DRenderState(grRenderBase* render):gr2DRenderStateBase(render) {}
};

#endif //2D_RENDER_STATE_H