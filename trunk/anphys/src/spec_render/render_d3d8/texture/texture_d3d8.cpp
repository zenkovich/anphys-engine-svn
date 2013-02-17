#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "texture_d3d8.h"
#include "render/render.h"
#include "render/texture_manager/texture_manager.h"

grTextureBase::grTextureBase(grTextureManager* textureManager):grTextureBaseInterface(textureManager)
{
	mTexturePtr = NULL;
}

grTextureBase::~grTextureBase()
{
	mTexturePtr->Release();
	grTextureBaseInterface::~grTextureBaseInterface();
}

bool grTextureBase::load(const std::string& fileName)
{
	if(FAILED(D3DXCreateTextureFromFile(mTextureManager->mRender->m_pDirect3DDevice, fileName.c_str(), &mTexturePtr)))
	{
		*mTextureManager->mRender->mRenderLog << formatStr("can't load texture %s\n", fileName.c_str()).c_str();
		mLoaded = false;
		mTexturePtr = NULL;
		return false;
	}

	mLoaded = true;
	mFileName = fileName;
	//get size
	return true;
}



#endif //RENDER_D3D8