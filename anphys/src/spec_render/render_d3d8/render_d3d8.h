#ifndef RENDER_D3D8_H
#define RENDER_D3D8_H

#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

#include <windows.h>

#include <string>

//utils
#include "../../util/math/mmath.h"

#include "render/render_base_interface.h"

#define D3DFVF_VERTEX_TEXTURE_LIGHT (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define D3DFVF_VERTEX_2D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2)
#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

#define DXCALL(call) if (FAILED(call)) assert(false, "DX Fail");

struct grRenderBase:public grRenderBaseInterface
{	
	LPDIRECT3D8           m_pDirect3d;
	LPDIRECT3DDEVICE8     m_pDirect3DDevice;
	D3DPRESENT_PARAMETERS mDirect3DParametr;

	LPDIRECT3DSURFACE8    mCurrentRenderTargetSurface;
	LPDIRECT3DSURFACE8    mCurrentRenderTargetDepthStencilSurface;

	bool              mReady;

//functions
	grRenderBase();
	grRenderBase(HWND HWnd, fRect drawRect);
	~grRenderBase();

	void initialize(HWND HWnd, fRect drawRect);

	void update(float dt);

	void beginRender();
	void render();
	void endRender();

	void resize(const vec2& size);

	void bindStencilBuffer(grStencilBufferRenderTarget* stencilBuffer);
	void unbindStencilBuffer(grStencilBufferRenderTarget* stencilBuffer = NULL);

	const char* getRenderName() { return "Direct3D8"; }
};

#endif //RENDER_D3D8_H