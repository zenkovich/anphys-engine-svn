#include "surface_material_base_interface.h"

#include "material.h"
#include "texture.h"
#include "render.h"
#include "surface_material_manager.h"
#include "texture_manager.h"


grSurfaceMaterialBaseInterface::~grSurfaceMaterialBaseInterface()
{
	removeAllTextures();
}

void grSurfaceMaterialBaseInterface::pushTexture(grTexture* texture)
{
	mTextures.push_back(texture);
}

void grSurfaceMaterialBaseInterface::setMaterial(grMaterial* material)
{
	mMaterial = material;
}

void grSurfaceMaterialBaseInterface::setShadeModel(grShadeModel* shadeModel)
{
	mShadeModel = shadeModel;
}

void grSurfaceMaterialBaseInterface::removeAllTextures()
{
	for (TexturesList::iterator it = mTextures.begin(); it != mTextures.end(); it++)
		mSurfaceMaterialManager->mRender->mTextures->removeTexture(*it);

	mTextures.clear();
}
