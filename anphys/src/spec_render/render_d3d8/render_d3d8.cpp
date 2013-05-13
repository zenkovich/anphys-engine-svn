#include "engine/engine_options.h"

#ifdef RENDER_D3D8

//render
#include "render_d3d8.h"
#include "render/scenes/scene_manager.h"
#include "render/render_target/backbuffer_render_target.h"
#include "render/render_target/stencil_buffer_render_target.h"
#include "render/render_state/render_state_interface.h"


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

	ZeroMemory(&mDirect3DParametr, sizeof(mDirect3DParametr));
	mDirect3DParametr.Windowed=true;
	mDirect3DParametr.SwapEffect = D3DSWAPEFFECT_COPY;
	mDirect3DParametr.BackBufferFormat = Display.Format;
	mDirect3DParametr.BackBufferCount = 1;
	mDirect3DParametr.BackBufferWidth = (unsigned int)(drawRect.rightDown.x-drawRect.leftTop.x);
	mDirect3DParametr.BackBufferHeight = (unsigned int)(drawRect.rightDown.y-drawRect.leftTop.y);  
	mDirect3DParametr.EnableAutoDepthStencil = true;
	mDirect3DParametr.AutoDepthStencilFormat = D3DFMT_D16;
	if(FAILED(m_pDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, HWnd, 
		      D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mDirect3DParametr, &m_pDirect3DDevice)))
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
	
	mBackbufferRenderTarget = new grBackbufferRenderTarget(this);
	mBackbufferRenderTarget->mSize = drawRect.getSize();
	mBackbufferRenderTarget->createDepthStencilSurface();

	mReady = true;
}

void grRenderBase::update(float dt)
{
	if (!mReady) return;

	grRenderBaseInterface::update(dt);
}

void grRenderBase::beginRender()
{
	if (!mReady) 
		return;

	//mLog->fout(1, "-------------------------------------\n");

	m_pDirect3DDevice->BeginScene();

	grRenderBaseInterface::beginRender();

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(220,255,220), 1.0F, 0);
}

void grRenderBase::render()
{
	if (!mReady) return;

	grRenderBaseInterface::render();
}

void grRenderBase::endRender()
{
	if (!mReady) return;

	grRenderBaseInterface::endRender();

	m_pDirect3DDevice->EndScene();
	m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}

void grRenderBase::resize( const vec2& size )
{
	//grRenderBaseInterface::resize(size);

	//safe_release(mBackbufferRenderTarget);

	mDirect3DParametr.BackBufferWidth = (UINT)size.x;
	mDirect3DParametr.BackBufferHeight = (UINT)size.y;

	/*HRESULT hr;
	if (FAILED(hr = m_pDirect3DDevice->Reset(&mDirect3DParametr)))
	{
		mLog->fout(1, "ERROR: Failed to reset direct 3d device");

		if (hr == D3DERR_OUTOFVIDEOMEMORY)
			mLog->fout(1, "error code: D3DERR_OUTOFVIDEOMEMORY");
		else if (hr == D3DERR_INVALIDCALL)
			mLog->fout(1, "error code: D3DERR_INVALIDCALL");
		else if (hr == E_OUTOFMEMORY)
			mLog->fout(1, "error code: E_OUTOFMEMORY");
		else 
			mLog->fout(1, "error code: %i", hr);
	}*/

	//mBackbufferRenderTarget = new grBackbufferRenderTarget(this);
	mBackbufferRenderTarget->mSize = size;
}

void grRenderBase::bindStencilBuffer( grStencilBufferRenderTarget* stencilBuffer )
{
	if (mCurrentRenderState)
		mCurrentRenderState->flush();

	mStencilBuffersStack.push_back(stencilBuffer);

	//mLog->fout(1, "grRenderBase::bindStencilBuffer setRenderTarger %x %x", mCurrentRenderTargetSurface, stencilBuffer->mDepthStencilSurface);
	DXCALL( m_pDirect3DDevice->SetRenderTarget(mCurrentRenderTargetSurface, stencilBuffer->mDepthStencilSurface) );
	DXCALL( m_pDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE) );
	DXCALL( m_pDirect3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL) );

	//mLog->fout(1, "Binded stencil %i %x", mStencilBuffersStack.size(), stencilBuffer);
}

void grRenderBase::unbindStencilBuffer(grStencilBufferRenderTarget* stencilBuffer)
{
	if (mCurrentRenderState)
		mCurrentRenderState->flush();

	//mLog->fout(1, "Unbind stencil %i %x", mStencilBuffersStack.size(), stencilBuffer);

	LPDIRECT3DSURFACE8 settingStencilBuffer = mCurrentRenderTargetDepthStencilSurface;
	if (stencilBuffer != NULL)
	{
		StencilBuffersList::iterator fnd = std::find(mStencilBuffersStack.begin(), mStencilBuffersStack.end(),
			stencilBuffer);

		mStencilBuffersStack.erase(fnd, mStencilBuffersStack.end());

		if (mStencilBuffersStack.size() != 0)
		{
			settingStencilBuffer = mStencilBuffersStack.back()->mDepthStencilSurface;
		}
	}
	else
	{
		settingStencilBuffer = mCurrentRenderTargetDepthStencilSurface;
		mStencilBuffersStack.clear();
		DXCALL( m_pDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE) );
	}

	if (mStencilBuffersStack.size() == 0)		
			DXCALL( m_pDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE) );

	//mLog->fout(1, "grRenderBase::unbindStencilBuffer setRenderTarger %x %x", mCurrentRenderTargetSurface, settingStencilBuffer);
	DXCALL( m_pDirect3DDevice->SetRenderTarget(mCurrentRenderTargetSurface, settingStencilBuffer) );
}

#endif //RENDER_D3D8
