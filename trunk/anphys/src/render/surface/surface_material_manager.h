#ifndef SURFACE_MATERIAL_MANAGER_H
#define SURFACE_MATERIAL_MANAGER_H

#include <vector>
#include <string>

struct grSurfaceMaterial;
struct grRender;
struct grRenderBaseInterface;

struct grSurfaceMaterialManager
{
	grRender* mRender;

	typedef std::vector<grSurfaceMaterial*> SurfaceMaterialsList;
	SurfaceMaterialsList mSurfaceMaterials;

	grSurfaceMaterialManager(grRenderBaseInterface* render);
	~grSurfaceMaterialManager();

	grSurfaceMaterial* addSurfaceMaterial(grSurfaceMaterial* material, bool canLoadMultiRef = true, bool willBeMultiRef = true);
	grSurfaceMaterial* createSurfaceMaterial(const std::string& materialFileName, bool canLoadMultiRef = true, bool willBeMultiRef = true) {}

	grSurfaceMaterial* getSurfaceMaterial(const std::string& name);

	bool removeSurfaceMaterial(grSurfaceMaterial* material);
	bool removeAllSurfaceMaterials();
};


#endif //SURFACE_MATERIAL_MANAGER_H