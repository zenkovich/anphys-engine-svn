#ifndef BACKBUFFER_RENDER_TARGET_D3D8_H
#define BACKBUFFER_RENDER_TARGET_D3D8_H

#include "render/render_target/backbuffer_render_target_base_interface.h"
#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

struct grBackbufferRenderTargetBase:public grBackbufferRenderTargetBaseInterface
{
	LPDIRECT3DSURFACE8 mRenderTargetSurface;
	LPDIRECT3DSURFACE8 mDepthStencilSurface;

	grBackbufferRenderTargetBase();
	grBackbufferRenderTargetBase(grRenderBase* render);

	bool begin();
};

#endif //BACKBUFFER_RENDER_TARGET_D3D8_H