#include "texture_manager.h"

#include <algorithm>

#include "render/render.h"
#include "texture.h"
#include "util/memory/mem_utils.h"
#include "util/log/log_system.h"
#include "util/log/log_stream_in_file.h"

grTextureManager::grTextureManager(grRenderBaseInterface* render)
{
	mRender = static_cast<grRender*>(render);

	mLog = gLogSystem->addStream(new cLogStreamInFile("tetxures_manager.txt"), "Textures");
	mLog->mLogLevel = INITIAL_TEXTURES_MANAGER_LOG_LEVEL;
}

grTextureManager::~grTextureManager()
{
	removeAllTextures();
	gLogSystem->removeStream(mLog);
}

grTexture* grTextureManager::addTexture(grTexture* texture)
{
	mTextures.push_back(texture);
	texture->mTextureManager = this;

	mLog->fout(1, "Add texture %x (%s)", texture, texture->mFileName.c_str());

	return *(mTextures.end() - 1);
}

grTexture* grTextureManager::createTexture(const std::string& textureFileName, bool canLoadMultiRef, bool willBeMultiRef)
{
	grTexture* newTexture = addTexture(new grTexture(this));

	if (canLoadMultiRef)
	{
		grTexture* tex = getTexture(textureFileName, false);

		if (tex) 
		{
			tex->mRefCount++;			
			mLog->fout(1, "Texture %x %s +ref = %i", tex, tex->mFileName.c_str(), tex->mRefCount);
			return tex;
		}
	}

	if (!newTexture->load(textureFileName)) 
	{
		mLog->fout(1, "Can't load texture: %s", textureFileName.c_str());
		return NULL;
	}
	newTexture->mCanCache = willBeMultiRef;

	mLog->fout(1, "Loaded texture: %s", textureFileName.c_str());
	return newTexture;
}
	
grTexture* grTextureManager::getTexture( const std::string& fileName, bool warnings /*= true*/ )
{
	for (TexturesList::iterator it = mTextures.begin(); it != mTextures.end(); it++)
		if ((*it)->mFileName == fileName) return (*it);

	if (warnings)
		mLog->fout(1, "Can't find texture %s", fileName.c_str());

	return NULL;
}

bool grTextureManager::removeTexture(grTexture* texture)
{
	TexturesList::iterator it = std::find(mTextures.begin(), mTextures.end(), texture);
	if (it == mTextures.end()) 
	{
		mLog->fout(1, "Can't find texture: %x", texture);
		return false;
	}

	if ((*it)->mRefCount < 2)
	{
		mLog->fout(1, "Removing texture %x %s", texture, texture->mFileName);
		safe_release(*it);
		mTextures.erase(it);
	}
	else
	{
		(*it)->mRefCount--;
		mLog->fout(1, "Texture %x %s -ref = %i", texture, texture->mFileName, texture->mRefCount);
	}

	return true;
}

bool grTextureManager::removeAllTextures()
{
	if (mTextures.size() == 0) return false;
 	for (TexturesList::iterator it = mTextures.begin(); it != mTextures.end(); it++)
		safe_release(*it);

	mLog->fout(1, "Removed all textures");

	mTextures.clear();
	return true;
}

void grTextureManager::processStreaming()
{
	if (!mUsingStreaming) return;

	float streamingTime = 0;
	const float maxStreamTime = 0.003f;

	for (TexturesList::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		if ((*it)->mLoaded) continue;

		(*it)->processStreaming(0);
	}
}
