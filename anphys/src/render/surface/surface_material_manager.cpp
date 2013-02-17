#include "surface_material_manager.h"

#include <algorithm>
#include "render/render.h"
#include "render/surface/surface_material.h"
#include "util/memory/mem_utils.h"


grSurfaceMaterialManager::grSurfaceMaterialManager(grRenderBaseInterface* render)
{	
	mRender = static_cast<grRender*>(render);
}

grSurfaceMaterialManager::~grSurfaceMaterialManager()
{
	removeAllSurfaceMaterials();
	*gLog << "grSurfaceTextureManager destructor\n";
}

grSurfaceMaterial* grSurfaceMaterialManager::addSurfaceMaterial(grSurfaceMaterial* material, bool canLoadMultiRef, bool willBeMultiRef)
{
	if (canLoadMultiRef)
	{
		grSurfaceMaterial* mat = getSurfaceMaterial(material->mName);

		if (mat)
		{
			mat->mRefCount++;
			*mRender->mRenderLog << formatStr("material ref +1 %x %s\n", mat, material->mName.c_str());
			return mat;
		}
	}

	mSurfaceMaterials.push_back(material);

	grSurfaceMaterial* newMaterial = *(mSurfaceMaterials.end() - 1);
	newMaterial->mSurfaceMaterialManager = this;
	newMaterial->mCanCache = willBeMultiRef;

	*mRender->mRenderLog << formatStr("add Surface material %x %s\n", newMaterial, newMaterial->mName.c_str());

	return newMaterial;
}

grSurfaceMaterial* grSurfaceMaterialManager::getSurfaceMaterial(const std::string& name)
{
	for (SurfaceMaterialsList::iterator it = mSurfaceMaterials.begin(); it != mSurfaceMaterials.end(); it++)
		if ((*it)->mName == name) return *it;

	*mRender->mRenderLog << formatStr("can't find Surface material %s\n", name.c_str());

	return NULL;
}

bool grSurfaceMaterialManager::removeSurfaceMaterial(grSurfaceMaterial* material)
{
	SurfaceMaterialsList::iterator it = std::find(mSurfaceMaterials.begin(), mSurfaceMaterials.end(), material);
	if (it == mSurfaceMaterials.end())
	{
		*mRender->mRenderLog << formatStr("can't remove Surface material %x %s\n", material, material->mName.c_str());
		return false;
	}

	if ((*it)->mRefCount < 2)
	{
		safe_release(*it);
		mSurfaceMaterials.erase(it);
	}
	else (*it)->mRefCount--;

	return true;
}

bool grSurfaceMaterialManager::removeAllSurfaceMaterials()
{
	if (mSurfaceMaterials.size() == 0) return false;
	for (SurfaceMaterialsList::iterator it = mSurfaceMaterials.begin(); it != mSurfaceMaterials.end(); it++)
		safe_release(*it);

	mSurfaceMaterials.clear();

	return true;
}
