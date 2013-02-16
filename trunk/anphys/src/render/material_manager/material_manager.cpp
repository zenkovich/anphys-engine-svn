#include "material_manager.h"

#include "render_base_interface.h"
#include "render.h"
#include "material.h"

#include <algorithm>

#include "../util/memory/mem_utils.h"


grMaterialManager::grMaterialManager(grRenderBaseInterface* render)
{	
	mRender = static_cast<grRender*>(render);
}

grMaterialManager::~grMaterialManager()
{
	removeAllMaterials();
	*gLog << "grTextureManager destructor\n";
}

grMaterial* grMaterialManager::addMaterial(grMaterial* material, bool canLoadMultiRef, bool willBeMultiRef)
{
	if (canLoadMultiRef)
	{
		grMaterial* mat = getMaterial(material->mName);

		if (mat)
		{
			mat->mRefCount++;
			*mRender->mRenderLog << formatStr("material ref +1 %x %s\n", mat, material->mName.c_str());
			return mat;
		}
	}

	mMaterials.push_back(material);

	grMaterial* newMaterial = *(mMaterials.end() - 1);
	newMaterial->mMaterialManager = this;
	newMaterial->mCanCache = willBeMultiRef;

	*mRender->mRenderLog << formatStr("add material %x %s\n", newMaterial, newMaterial->mName.c_str());

	return newMaterial;
}

grMaterial* grMaterialManager::getMaterial(const std::string& name)
{
	for (MaterialsList::iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
		if ((*it)->mName == name) return *it;

	*mRender->mRenderLog << formatStr("can't find material %s\n", name.c_str());

	return NULL;
}

bool grMaterialManager::removeMaterial(grMaterial* material)
{
	MaterialsList::iterator it = std::find(mMaterials.begin(), mMaterials.end(), material);
	if (it == mMaterials.end())
	{
		*mRender->mRenderLog << formatStr("can't remove material %x %s\n", material, material->mName.c_str());
		return false;
	}

	if ((*it)->mRefCount < 2)
	{
		safe_release(*it);
		mMaterials.erase(it);
	}
	else (*it)->mRefCount--;

	return true;
}

bool grMaterialManager::removeAllMaterials()
{
	if (mMaterials.size() == 0) return false;
	for (MaterialsList::iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
		safe_release(*it);

	mMaterials.clear();

	return true;
}
