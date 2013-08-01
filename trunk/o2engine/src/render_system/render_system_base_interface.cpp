#include "render_system_base_interface.h"

#include <algorithm>

#include "texture.h"
#include "app/application.h"
#include "texture.h"

OPEN_O2_NAMESPACE

grRenderSystemBaseInterface::grRenderSystemBaseInterface( cApplication* application ):
	mCurrentCamera(NULL), mApplication(application)
{
}

grRenderSystemBaseInterface::~grRenderSystemBaseInterface()
{
	removeAllTextures();
}

bool grRenderSystemBaseInterface::bindCamera( grCamera* camera )
{
	mCurrentCamera = camera;
	updateCameraTransforms();

	return true;
}

grTexture* grRenderSystemBaseInterface::addTexture( grTexture* texture )
{
	mTextures.push_back(texture);
	return texture;
}

grTexture* grRenderSystemBaseInterface::createTexture( const std::string& fileName )
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		if (fileName == (*it)->getFileName())
		{
			(*it)->incRefCount();
			return *it;
		}
	}

	grTexture* newTexture = new grTexture((grRenderSystem*)this, fileName);
	newTexture->incRefCount();
	return addTexture(newTexture);
}

bool grRenderSystemBaseInterface::removeTexture( grTexture* texture )
{
	texture->decrRefCount();

	if (texture->getRefCount() == 0)
	{
		bool inVec = false;

		TexturesVec::iterator fnd = std::find(mTextures.begin(), mTextures.end(), texture);
		if (fnd != mTextures.end())
		{
			mTextures.erase(fnd);
			inVec = true;
		}

		safe_release(texture);

		return inVec;
	}

	return true;
}

bool grRenderSystemBaseInterface::removeAllTextures()
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
		safe_release(*it);

	mTextures.clear();

	return true;
}

CLOSE_O2_NAMESPACE