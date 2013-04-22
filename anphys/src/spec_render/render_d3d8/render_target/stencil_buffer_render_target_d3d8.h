#ifndef STENCIL_BUFFER_RENDER_TARGET_D3D8_H
#define STENCIL_BUFFER_RENDER_TARGET_D3D8_H

#include "render/render_target/stencil_buffer_render_target_base_interface.h"
#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

struct grStencilBufferRenderTargetBase:public grStencilBufferRenderTargetBaseInterface
{
	LPDIRECT3DSURFACE8 mDepthStencilSurface;

//functions
	grStencilBufferRenderTargetBase();
	grStencilBufferRenderTargetBase(grRenderBase* render);
	grStencilBufferRenderTargetBase(grRenderBase* render, const vec2& size);
	virtual ~grStencilBufferRenderTargetBase();

	bool begin();
	bool finish();

	void clear();
	void fillRect(const fRect& rect);

	void createSurface();
};

#endif //STENCIL_BUFFER_RENDER_TARGET_D3D8_H