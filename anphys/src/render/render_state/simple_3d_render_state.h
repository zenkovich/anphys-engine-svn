#ifndef SIMPLE_3D_RENDER_STATE_H
#define SIMPLE_3D_RENDER_STATE_H

#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/render_state/simple_3d_render_state_d3d8.h"
#endif //RENDER_D3D8

struct grSimple3DRenderState:public grSimple3DRenderStateBase
{
	grSimple3DRenderState():grSimple3DRenderStateBase() {}
	grSimple3DRenderState(grRenderBase* render):grSimple3DRenderStateBase(render) {}
};

#endif //SIMPLE_3D_RENDER_STATE_H