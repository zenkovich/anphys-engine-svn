#include "engine/engine_options.h"
#ifdef RENDER_D3D8

#include "texture_render_target_d3d8.h"

#include "render/render.h"
#include "render/texture_manager/texture.h"


bool grTextureRenderTargetBase::begin()
{
	if (!mTexture->mTexturePtr) return true;

	mSize = mTexture->mSize;

	//mRender->mRender2D->drawPrimitives();

	LPDIRECT3DSURFACE8 renderTargetSurface;
	LPDIRECT3DTEXTURE8 renderTargetTexture = mTexture->mTexturePtr;
	if (FAILED(renderTargetTexture->GetSurfaceLevel(0, &renderTargetSurface)))
	{
		mRender->mLog->fout(1, "ERROR: Can't get surface level 0 from texture %x", mTexture);
		return false;
	}
	if (renderTargetSurface) renderTargetSurface->Release();

	//mRender->mLog->fout(1, "grTextureRenderTargetBase::begin setRenderTarger %x %x", renderTargetSurface, mRender->mCurrentRenderTargetDepthStencilSurface);
	if (FAILED(mRender->m_pDirect3DDevice->SetRenderTarget(renderTargetSurface, mRender->mCurrentRenderTargetDepthStencilSurface)))
	{
		mRender->mLog->fout(1, "ERROR: Can't setup render target %x: failed SetRenderTarget", this);
		return false;
	}

	mRender->mCurrentRenderTargetSurface = renderTargetSurface;

	mRender->m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(200, 200, 200, 255), 1.0f, 0);

	return true;
}

bool grTextureRenderTargetBase::finish()
{
//	mRender->mRender2D->drawPrimitives();

	return true;
}

#endif //RENDER_D3D8
