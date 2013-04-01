#ifndef TEXTURE_RENDER_TARGET_D3D8_H
#define TEXTURE_RENDER_TARGET_D3D8_H

#include "render/render_target/texture_render_target_base_interface.h"

struct grTextureRenderTargetBase:public grTextureRenderTargetInterface
{
	grTextureRenderTargetBase():grTextureRenderTargetInterface() {}
	grTextureRenderTargetBase(grRenderBase* render):grTextureRenderTargetInterface(render) {}
	grTextureRenderTargetBase(grRenderBase* render, grTexture* texture):grTextureRenderTargetInterface(render, texture) {}

	bool begin();
	bool finish();
};

#endif //TEXTURE_RENDER_TARGET_D3D8_H