#include "material_manager.h"

#include <algorithm>

#include "render/render.h"
#include "material.h"
#include "util/memory/mem_utils.h"
#include "util/log/log_stream.h"
#include "util/log/log_system.h"


grMaterialManager::grMaterialManager(grRenderBaseInterface* render)
{	
	mRender = static_cast<grRender*>(render);
	mLog = gLogSystem->addStream(new cLogStreamInFile("materials_log.txt"), "MaterialsLog");
	mLog->mLogLevel = STD_LOG_LEVEL;
}

grMaterialManager::~grMaterialManager()
{
	removeAllMaterials();
	gLogSystem->removeStream(mLog);
}

grMaterial* grMaterialManager::addMaterial(grMaterial* material, bool canLoadMultiRef, bool willBeMultiRef)
{
	if (canLoadMultiRef)
	{
		grMaterial* mat = getMaterial(material->mName, false);

		if (mat)
		{
			mat->mRefCount++;
			mLog->fout(1, "Material ref +1 = %i %x %s", mat->mRefCount, mat, material->mName.c_str());
			return mat;
		}
	}

	mMaterials.push_back(material);

	grMaterial* newMaterial = *(mMaterials.end() - 1);
	newMaterial->mMaterialManager = this;
	newMaterial->mCanCache = willBeMultiRef;

	mLog->fout(1, "Add material %x %s", newMaterial, newMaterial->mName.c_str());

	return newMaterial;
}

grMaterial* grMaterialManager::getMaterial( const std::string& name, bool warnings /*= true*/ )
{
	for (MaterialsList::iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
		if ((*it)->mName == name) return *it;

	if (warnings)
		mLog->fout(1, "Can't find material %s", name.c_str());

	return NULL;
}

bool grMaterialManager::removeMaterial(grMaterial* material)
{
	MaterialsList::iterator it = std::find(mMaterials.begin(), mMaterials.end(), material);
	if (it == mMaterials.end())
	{
		mLog->fout(1, "Can't remove material %x %s", material, material->mName.c_str());
		return false;
	}

	if ((*it)->mRefCount < 1)
	{
		mLog->fout(1, "Removing material %x %s", *it, (*it)->mName.c_str());

		safe_release(*it);
		mMaterials.erase(it);
	}
	else
	{
		(*it)->mRefCount--;

		mLog->fout(1, "Material %x -ref = %i", *it, (*it)->mRefCount);
	}

	return true;
}

bool grMaterialManager::removeAllMaterials()
{
	if (mMaterials.size() == 0) return false;
	for (MaterialsList::iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
		safe_release(*it);

	mMaterials.clear();

	mLog->fout(1, "Removed all materials");

	return true;
}
