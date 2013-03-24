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
#define D3DFVF_VERTEX_2D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct grRenderBase:public grRenderBaseInterface
{	
	LPDIRECT3D8       m_pDirect3d;
	LPDIRECT3DDEVICE8 m_pDirect3DDevice;

	bool              mReady;

//functions
	grRenderBase();
	grRenderBase(HWND HWnd, fRect drawRect);
	~grRenderBase();

	void initialize(HWND HWnd, fRect drawRect);

	void update(float dt);

	void preRender();
	void render();
	void postRender();

	const char* getRenderName() { return "Direct3D8"; }
};

#endif //RENDER_D3D8_H