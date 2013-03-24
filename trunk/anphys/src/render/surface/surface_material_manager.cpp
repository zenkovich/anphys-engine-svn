#include "surface_material_manager.h"

#include <algorithm>
#include "render/render.h"
#include "render/surface/surface_material.h"
#include "util/memory/mem_utils.h"


grSurfaceMaterialManager::grSurfaceMaterialManager(grRenderBaseInterface* render)
{	
	mRender = static_cast<grRender*>(render);
	mLog = gLogSystem->addStream(new cLogStreamInFile("surface_material_manager.txt"), "Surfaces");
	mLog->mLogLevel = INITIAL_TEXTURES_MANAGER_LOG_LEVEL;
}

grSurfaceMaterialManager::~grSurfaceMaterialManager()
{
	removeAllSurfaceMaterials();
	gLogSystem->removeStream(mLog);
}

grSurfaceMaterial* grSurfaceMaterialManager::addSurfaceMaterial(grSurfaceMaterial* material, bool canLoadMultiRef, bool willBeMultiRef)
{
	if (canLoadMultiRef)
	{
		grSurfaceMaterial* mat = getSurfaceMaterial(material->mName, false);

		if (mat)
		{
			mat->mRefCount++;
			mLog->fout(1, "Surface ref +1 = %i %x %s", mat->mRefCount, mat, material->mName.c_str());
			return mat;
		}
	}

	mSurfaceMaterials.push_back(material);

	grSurfaceMaterial* newMaterial = *(mSurfaceMaterials.end() - 1);
	newMaterial->mSurfaceMaterialManager = this;
	newMaterial->mCanCache = willBeMultiRef;

	mLog->fout(1, "Add material %x %s", newMaterial, newMaterial->mName.c_str());

	return newMaterial;
}

grSurfaceMaterial* grSurfaceMaterialManager::getSurfaceMaterial( const std::string& name, bool warnings /*= true*/ )
{
	for (SurfaceMaterialsList::iterator it = mSurfaceMaterials.begin(); it != mSurfaceMaterials.end(); it++)
		if ((*it)->mName == name) return *it;

	if (warnings)
		mLog->fout(1, "Can't find surface material %s", name.c_str());

	return NULL;
}

bool grSurfaceMaterialManager::removeSurfaceMaterial(grSurfaceMaterial* material)
{
	SurfaceMaterialsList::iterator it = std::find(mSurfaceMaterials.begin(), mSurfaceMaterials.end(), material);
	if (it == mSurfaceMaterials.end())
	{
		mLog->fout(1, "Can't remove surface material %x %s", material, material->mName.c_str());
		return false;
	}

	if ((*it)->mRefCount < 1)
	{
		mLog->fout(1, "Removing surface material %x %s", material, material->mName.c_str());
		safe_release(*it);
		mSurfaceMaterials.erase(it);
	}
	else 
	{
		mLog->fout(1, "Decraise surface material %x %s -ref = %i", material, material->mName.c_str(), material->mRefCount);
		(*it)->mRefCount--;
	}

	return true;
}

bool grSurfaceMaterialManager::removeAllSurfaceMaterials()
{
	if (mSurfaceMaterials.size() == 0) return false;
	for (SurfaceMaterialsList::iterator it = mSurfaceMaterials.begin(); it != mSurfaceMaterials.end(); it++)
		safe_release(*it);

	mSurfaceMaterials.clear();

	mLog->fout(1, "Removed all surface materials");

	return true;
}
