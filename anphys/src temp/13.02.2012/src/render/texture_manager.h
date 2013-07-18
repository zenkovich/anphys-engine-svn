#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <vector>
#include <string>

struct grTexture;
struct grRender;
struct grRenderBaseInterface;

struct grTextureManager
{
	grRender* mRender;

	typedef std::vector<grTexture*> TexturesList;
	TexturesList mTextures;

	grTextureManager(grRenderBaseInterface* render);
	~grTextureManager();

	grTexture* addTexture(grTexture* texture);
	grTexture* createTexture(const std::string& textureFileName, bool canLoadMultiRef = true, bool willBeMultiRef = true);

	grTexture* getTexture(const std::string& fileName);

	bool removeTexture(grTexture* texture);
	bool removeAllTextures();
};

#endif //TEXTURE_MANAGER_H