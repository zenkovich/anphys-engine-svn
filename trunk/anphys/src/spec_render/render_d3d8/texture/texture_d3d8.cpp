#include "engine/engine_options.h"

#ifdef RENDER_D3D8

//render
#include "texture_d3d8.h"
#include "render/render.h"
#include "render/texture_manager/texture_manager.h"

//utils
#include "util/image/image.h"


grTextureBase::grTextureBase(grTextureManager* textureManager):grTextureBaseInterface(textureManager), mTexturePtr(NULL),
	mSourceImage(NULL), mLastStreamedLine(0), mUploadedDataSize(0)
{
}

grTextureBase::~grTextureBase()
{
	mTexturePtr->Release();

	safe_release(mSourceImage);

	grTextureBaseInterface::~grTextureBaseInterface();
}

bool grTextureBase::load(const std::string& fileName)
{
	mSourceImage = new cImage;
	if (mSourceImage->load(fileName.c_str(), cImage::IT_PNG))
	{
		mSize = mSourceImage->mSize;

		if (mTextureManager && mTextureManager->mUsingStreaming)
		{
			mLoaded = false;
			mLastStreamedLine = 0;
			mUploadedDataSize = 0;
		}
	}
	else safe_release(mSourceImage);

	mFileName = fileName;

	return true;
}

void grTextureBase::processStreaming( unsigned int maxStreamingData )
{
	if (!mSourceImage)
	{		
		if (FAILED(D3DXCreateTextureFromFile(mTextureManager->mRender->m_pDirect3DDevice, mFileName.c_str(), &mTexturePtr)))
		{
			*mTextureManager->mRender->mRenderLog << formatStr("ERROR: Can't load texture '%s'\n", mFileName.c_str());
			mLoaded = true;
			return;
		}
		else
		{
			mLoaded = true;
			D3DSURFACE_DESC desc;
			mTexturePtr->GetLevelDesc(0, &desc);
			mSize = vec2((float)desc.Width, (float)desc.Height);
		}
	}
	else
	{
		if (!mTexturePtr)
		{
			if (FAILED(mTextureManager->mRender->m_pDirect3DDevice->CreateTexture((UINT)mSourceImage->mSize.x, (UINT)mSourceImage->mSize.y, 1, 
				0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &mTexturePtr)))
			{
				*mTextureManager->mRender->mRenderLog << formatStr("ERROR: Can't create texture %ix%i D3DFMT_A8R8G8B8",
					(UINT)mSourceImage->mSize.x, (UINT)mSourceImage->mSize.y);
				mLoaded = true;
				return;
			}
		}

		D3DLOCKED_RECT lockingRect;
		if ( FAILED(mTexturePtr->LockRect(0, &lockingRect, 0, D3DLOCK_DISCARD)) )
		{
			*mTextureManager->mRender->mRenderLog << formatStr("ERROR: Failed to lock texture rect '%s'", mFileName.c_str());
			mLoaded = true;
			return;
		}


	}
}



#endif //RENDER_D3D8