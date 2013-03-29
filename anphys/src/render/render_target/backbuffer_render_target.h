#ifndef BACKBUFFER_RENDER_TARGET_H
#define BACKBUFFER_RENDER_TARGET_H

#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/render_target/backbuffer_render_target_d3d8.h"
#endif //RENDER_D3D8

struct grBackbufferRenderTarget:public grBackbufferRenderTargetBase
{
	grBackbufferRenderTarget():grBackbufferRenderTargetBase() {}
	grBackbufferRenderTarget(grRenderBase* render):grBackbufferRenderTargetBase(render) {}
};


#endif //BACKBUFFER_RENDER_TARGET_H