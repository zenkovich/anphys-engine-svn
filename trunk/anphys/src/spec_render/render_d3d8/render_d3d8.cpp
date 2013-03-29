#include "engine/engine_options.h"

#ifdef RENDER_D3D8

//render
#include "render_d3d8.h"
#include "render/scenes/scene_manager.h"
#include "render/2d_render/render_2d.h"
#include "render/render_target/backbuffer_render_target.h"


grRenderBase::grRenderBase():grRenderBaseInterface(), m_pDirect3d(NULL), m_pDirect3DDevice(NULL)
{
	mReady = false;
}

grRenderBase::grRenderBase(HWND HWnd, fRect drawRect)
{
	initialize(HWnd, drawRect);
}

grRenderBase::~grRenderBase()
{
	m_pDirect3DDevice->Release();
	m_pDirect3d->Release();
}

void grRenderBase::initialize(HWND HWnd, fRect drawRect)
{
	mReady = false;

		mLog->fout(1, "Initializing Direct3d8 render...");

	if(NULL == (m_pDirect3d = Direct3DCreate8(D3D_SDK_VERSION)))
	{
		mLog->fout(1, "ERROR: Direct3DCreate8 failed!");
		return;
	}

	D3DDISPLAYMODE Display;
	if(FAILED(m_pDirect3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display)))
	{
		mLog->fout(1, "ERROR: GetAdapterDisplayMode failed");
		return;
	}

	D3DPRESENT_PARAMETERS Direct3DParametr;
	ZeroMemory(&Direct3DParametr, sizeof(Direct3DParametr));
	Direct3DParametr.Windowed=true;
	Direct3DParametr.SwapEffect = D3DSWAPEFFECT_COPY;
	Direct3DParametr.BackBufferFormat = Display.Format;
	Direct3DParametr.BackBufferHeight = (UINT)(int)(drawRect.rightDown.x-drawRect.leftTop.x);
	Direct3DParametr.BackBufferWidth = (UINT)(int)(drawRect.rightDown.y-drawRect.leftTop.y);  
	Direct3DParametr.EnableAutoDepthStencil = true;
	Direct3DParametr.AutoDepthStencilFormat = D3DFMT_D16;
	if(FAILED(m_pDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, HWnd, 
		      D3DCREATE_HARDWARE_VERTEXPROCESSING, &Direct3DParametr, &m_pDirect3DDevice)))
	{
		mLog->fout(1, "ERROR: CreateDevice failed");
		return ;
	}
	m_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pDirect3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	
	
    m_pDirect3DDevice->SetRenderState(D3DRS_DITHERENABLE, true);
    m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);
	
    m_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	mLog->fout(1, "Direct3d8 render initialized");

	mRender2D->initialize();

	mBackbufferRenderTarget = new grBackbufferRenderTarget(this);

	mReady = true;
}

void grRenderBase::update(float dt)
{
	if (!mReady) return;

	grRenderBaseInterface::update(dt);
}

void grRenderBase::preRender()
{
	if (!mReady) return;

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(220,255,220), 1.0F, 0);
	m_pDirect3DDevice->BeginScene();

	grRenderBaseInterface::preRender();
}

void grRenderBase::render()
{
	if (!mReady) return;

	grRenderBaseInterface::render();
}

void grRenderBase::postRender()
{
	if (!mReady) return;

	grRenderBaseInterface::postRender();

	m_pDirect3DDevice->EndScene();
	m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}

#endif //RENDER_D3D8
