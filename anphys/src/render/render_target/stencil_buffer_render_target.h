#ifndef STENCIL_BUFFER_RENDER_TARGET_H
#define STENCIL_BUFFER_RENDER_TARGET_H

#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/render_target/stencil_buffer_render_target_d3d8.h"
#endif

struct grStencilBufferRenderTarget:public grStencilBufferRenderTargetBase
{
	grStencilBufferRenderTarget():grStencilBufferRenderTargetBase() {}
	grStencilBufferRenderTarget(grRenderBase* render):grStencilBufferRenderTargetBase(render) {}
	grStencilBufferRenderTarget(grRenderBase* render, const vec2& size):
		grStencilBufferRenderTargetBase(render) {}
};

#endif //STENCIL_BUFFER_RENDER_TARGET_H