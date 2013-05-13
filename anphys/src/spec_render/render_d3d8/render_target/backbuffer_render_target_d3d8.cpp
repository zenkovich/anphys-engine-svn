#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "backbuffer_render_target_d3d8.h"
#include "render/render.h"

grBackbufferRenderTargetBase::grBackbufferRenderTargetBase():mRenderTargetSurface(NULL), mDepthStencilSurface(NULL)
{
}

grBackbufferRenderTargetBase::grBackbufferRenderTargetBase( grRenderBase* render ):grBackbufferRenderTargetBaseInterface(render)
{
	if (FAILED(mRender->m_pDirect3DDevice->GetRenderTarget(&mRenderTargetSurface)))
		render->mLog->fout(1, "ERROR: Can't initialize backbuffer render target: GetRenderTarget failed");

	//createDepthStencilSurface();

	/*if (FAILED(mRender->m_pDirect3DDevice->GetDepthStencilSurface(&mDepthStencilSurface)))
		render->mLog->fout(1, "ERROR: Can't initialize backbuffer render target: GetDepthStencilSurface failed");*/
}

bool grBackbufferRenderTargetBase::begin()
{
	//mRender->mLog->fout(1, "grBackbufferRenderTargetBase::begin setRenderTarger %x %x", mRenderTargetSurface, mDepthStencilSurface);
	
	bool res = !(FAILED(mRender->m_pDirect3DDevice->SetRenderTarget(mRenderTargetSurface, mDepthStencilSurface)));
	DXCALL( mRender->m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_STENCIL, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1) );
	//DXCALL( mRender->m_pDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE) );

	if (res)
	{
		mRender->mCurrentRenderTargetSurface = mRenderTargetSurface;
		mRender->mCurrentRenderTargetDepthStencilSurface = mDepthStencilSurface;
	}

	return res;
}

void grBackbufferRenderTargetBase::createDepthStencilSurface()
{	
	mRender->m_pDirect3DDevice->CreateDepthStencilSurface((UINT)mSize.x, (UINT)mSize.y, D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, &mDepthStencilSurface);
}

#endif //RENDER_D3D8
