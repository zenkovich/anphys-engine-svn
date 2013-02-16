#ifndef RENDER_D3D8_H
#define RENDER_D3D8_H

#include <d3d8.h>
#include <d3dx8.h>
#include <d3dx8core.h>

#include <windows.h>

#include <string>

//utils
#include "../../util/math/mmath.h"

#include "../render_base_interface.h"

struct grRenderBase:public grRenderBaseInterface
{	
	//d3d
	LPDIRECT3D8 m_pDirect3d;
	LPDIRECT3DDEVICE8 m_pDirect3DDevice;

	bool mReady;

	///-----------------------------------------///
	grRenderBase();
	grRenderBase(HWND HWnd, fRect drawRect);
	~grRenderBase();

	void initialize(HWND HWnd, fRect drawRect);

	void update(float dt);

	void preRender();
	void render();
	void postRender();

	std::string getRenderName() { return "Direct3D8"; }
};

#endif //RENDER_D3D8_H