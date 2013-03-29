#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <vector>
#include <string>
#include "util/math/mmath.h"

struct grTexture;
struct grRender;
struct grRenderBaseInterface;
struct cLogStream;

struct grTextureManager
{
	typedef std::vector<grTexture*> TexturesList;

	grRender*    mRender;
	TexturesList mTextures;
	bool         mUsingStreaming;
	cLogStream*  mLog;

//functions
	grTextureManager(grRenderBaseInterface* render);
	~grTextureManager();

	grTexture* addTexture(grTexture* texture);
	grTexture* createTexture(const std::string& textureFileName, bool canLoadMultiRef = true, bool willBeMultiRef = true);
	grTexture* createRenderTexture(const vec2& size);

	grTexture* getTexture(const std::string& fileName, bool warnings = true);

	bool removeTexture(grTexture* texture);
	bool removeAllTextures();

	void processStreaming();
};

#endif //TEXTURE_MANAGER_H