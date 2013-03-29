#ifndef TEXTURE_RENDER_TARGET_H
#define TEXTURE_RENDER_TARGET_H

#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/render_target/texture_render_target_d3d8.h"
#endif

struct grTextureRenderTarget:public grTextureRenderTargetBase
{
	grTextureRenderTarget():grTextureRenderTargetBase() {}
	grTextureRenderTarget(grRenderBase* render):grTextureRenderTargetBase(render) {}
	grTextureRenderTarget(grRenderBase* render, grTexture* texture):grTextureRenderTargetBase(render, texture) {}
};

#endif //TEXTURE_RENDER_TARGET_H