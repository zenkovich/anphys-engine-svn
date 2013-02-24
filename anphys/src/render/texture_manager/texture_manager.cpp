#include "texture_manager.h"

#include <algorithm>

#include "render/render.h"
#include "texture.h"
#include "util/memory/mem_utils.h"

grTextureManager::grTextureManager(grRenderBaseInterface* render)
{
	mRender = static_cast<grRender*>(render);
}

grTextureManager::~grTextureManager()
{
	removeAllTextures();
	*gLog << "grTextureManager destructor\n";
}

grTexture* grTextureManager::addTexture(grTexture* texture)
{
	mTextures.push_back(texture);
	texture->mTextureManager = this;
	*mRender->mRenderLog << formatStr("added texture %x\n", texture);
	return *(mTextures.end() - 1);
}

grTexture* grTextureManager::createTexture(const std::string& textureFileName, bool canLoadMultiRef, bool willBeMultiRef)
{
	grTexture* newTexture = addTexture(new grTexture(this));

	if (canLoadMultiRef)
	{
		grTexture* tex = getTexture(textureFileName);

		if (tex) 
		{
			tex->mRefCount++;			
			*mRender->mRenderLog << formatStr("texture ref +1 %x\n", tex);
			return tex;
		}
	}

	if (!newTexture->load(textureFileName)) 
	{
		*mRender->mRenderLog << formatStr("can't load texture: %s\n", textureFileName.c_str());
		return NULL;
	}
	newTexture->mCanCache = willBeMultiRef;

	*mRender->mRenderLog << formatStr("loaded texture: %s\n", textureFileName.c_str());
	return newTexture;
}
	
grTexture* grTextureManager::getTexture(const std::string& fileName)
{
	for (TexturesList::iterator it = mTextures.begin(); it != mTextures.end(); it++)
		if ((*it)->mFileName == fileName) return (*it);

	return NULL;
}

bool grTextureManager::removeTexture(grTexture* texture)
{
	TexturesList::iterator it = std::find(mTextures.begin(), mTextures.end(), texture);
	if (it == mTextures.end()) 
	{
		*mRender->mRenderLog << formatStr("can't find texture: %x\n", texture);
		return false;
	}

	if ((*it)->mRefCount < 2)
	{
		safe_release(*it);
		mTextures.erase(it);
	}
	else (*it)->mRefCount--;

	return true;
}

bool grTextureManager::removeAllTextures()
{
	if (mTextures.size() == 0) return false;
 	for (TexturesList::iterator it = mTextures.begin(); it != mTextures.end(); it++)
		safe_release(*it);

	mTextures.clear();
	return true;
}
