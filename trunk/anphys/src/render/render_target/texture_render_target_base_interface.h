#ifndef TEXTURE_RENDER_TARGET_BASE_INTERFACE_H
#define TEXTURE_RENDER_TARGET_BASE_INTERFACE_H

#include "render_target_interface.h"

struct grTexture;

struct grTextureRenderTargetInterface:public grRenderTarget
{
	grTexture* mTexture;

	grTextureRenderTargetInterface():grRenderTarget(), mTexture(0) {}
	grTextureRenderTargetInterface(grRenderBase* render):grRenderTarget(render), mTexture(0) {}
	grTextureRenderTargetInterface(grRenderBase* render, grTexture* texture):grRenderTarget(render), mTexture(texture) {}
	~grTextureRenderTargetInterface() {}

	virtual bool begin() { return true; }
	virtual bool finish() { return true; }
};

#endif //TEXTURE_RENDER_TARGET_BASE_INTERFACE_H