#include "engine/engine_options.h"
#ifdef RENDER_D3D8

#include "texture_render_target_d3d8.h"

#include "render/render.h"
#include "render/texture_manager/texture.h"


bool grTextureRenderTargetBase::setup()
{
	if (!mTexture->mTexturePtr) return true;

	LPDIRECT3DSURFACE8 renderTargetSurface;
	LPDIRECT3DTEXTURE8 renderTargetTexture = mTexture->mTexturePtr;
	if (FAILED(renderTargetTexture->GetSurfaceLevel(0, &renderTargetSurface)))
	{
		mRender->mLog->fout(1, "ERROR: Can't get surface level 0 from texture %x", mTexture);
		return false;
	}
	if (renderTargetSurface) renderTargetSurface->Release();

	if (FAILED(mRender->m_pDirect3DDevice->SetRenderTarget(renderTargetSurface, NULL)))
	{
		mRender->mLog->fout(1, "ERROR: Can't setup render target %x: failed SetRenderTarget", this);
		return false;
	}

	mRender->m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0F, 0);

	return true;
}

#endif //RENDER_D3D8
