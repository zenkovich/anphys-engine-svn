#include "stencil_buffer_render_target_d3d8.h"

#include "render/render.h"
#include "render/render_target/backbuffer_render_target.h"

grStencilBufferRenderTargetBase::grStencilBufferRenderTargetBase():grStencilBufferRenderTargetBaseInterface()
{
}

grStencilBufferRenderTargetBase::grStencilBufferRenderTargetBase( grRenderBase* render ):
	grStencilBufferRenderTargetBaseInterface(render)
{
	mSize = mRender->mBackbufferRenderTarget->mSize;
	createSurface();
}

grStencilBufferRenderTargetBase::grStencilBufferRenderTargetBase( grRenderBase* render, const vec2& size ):
	grStencilBufferRenderTargetBaseInterface(render, size)
{
	createSurface();
}

grStencilBufferRenderTargetBase::~grStencilBufferRenderTargetBase()
{
	mDepthStencilSurface->Release();
}

void grStencilBufferRenderTargetBase::createSurface()
{
	DXCALL( mRender->m_pDirect3DDevice->CreateDepthStencilSurface((UINT)mSize.x, (UINT)mSize.y, D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, &mDepthStencilSurface) );
}

bool grStencilBufferRenderTargetBase::begin()
{
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE) );
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS) );
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILREF, 0x1) );
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff) );
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff) );
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP) );
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP) );
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE) );

	DWORD rsColorWriteFlag = 0;

	//mRender->mLog->fout(1, "grStencilBufferRenderTargetBase::begin setRenderTarger %x %x", mRender->mCurrentRenderTargetSurface, mDepthStencilSurface);
	DXCALL( mRender->m_pDirect3DDevice->SetRenderTarget(mRender->mCurrentRenderTargetSurface, mDepthStencilSurface) );
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, rsColorWriteFlag) );

	return true;
}

bool grStencilBufferRenderTargetBase::finish()
{
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE) );

	DWORD rsColorWriteFlag = D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		                     D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA;
	DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, rsColorWriteFlag) );

	/*mRender->mLog->fout(1, "grStencilBufferRenderTargetBase::finish setRenderTarger %x %x", mRender->mCurrentRenderTargetSurface,
		mRender->mCurrentRenderTargetDepthStencilSurface);*/
	DXCALL( mRender->m_pDirect3DDevice->SetRenderTarget(mRender->mCurrentRenderTargetSurface,
		mRender->mCurrentRenderTargetDepthStencilSurface) );

	return true;
}

void grStencilBufferRenderTargetBase::clear()
{
	DXCALL( mRender->m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0) );
}

void grStencilBufferRenderTargetBase::fillRect( const fRect& rect )
{
	D3DRECT rt;
	rt.x1 = (LONG)rect.leftTop.x; rt.y1 = (LONG)rect.leftTop.y;
	rt.x2 = (LONG)rect.rightDown.x; rt.y2 = (LONG)rect.rightDown.y;

	DXCALL( mRender->m_pDirect3DDevice->Clear(1, &rt, D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 1) );
}
