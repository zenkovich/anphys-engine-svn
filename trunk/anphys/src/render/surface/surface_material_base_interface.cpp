#include "surface_material_base_interface.h"

#include "render/material_manager/material.h"
#include "render/texture_manager/texture_manager.h"
#include "render/texture_manager/texture.h"
#include "render/render.h"
#include "surface_material_manager.h"


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
