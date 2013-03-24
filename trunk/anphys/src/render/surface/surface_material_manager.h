#ifndef SURFACE_MATERIAL_MANAGER_H
#define SURFACE_MATERIAL_MANAGER_H

#include <vector>
#include <string>

struct grSurfaceMaterial;
struct grRender;
struct grRenderBaseInterface;
struct cLogStream;

struct grSurfaceMaterialManager
{
	typedef std::vector<grSurfaceMaterial*> SurfaceMaterialsList;

	grRender*            mRender;
	SurfaceMaterialsList mSurfaceMaterials;
	cLogStream*          mLog;

//functions
	grSurfaceMaterialManager(grRenderBaseInterface* render);
	~grSurfaceMaterialManager();

	grSurfaceMaterial* addSurfaceMaterial(grSurfaceMaterial* material, bool canLoadMultiRef = true, bool willBeMultiRef = true);
	grSurfaceMaterial* createSurfaceMaterial(const std::string& materialFileName, bool canLoadMultiRef = true, bool willBeMultiRef = true) {}

	grSurfaceMaterial* getSurfaceMaterial(const std::string& name, bool warnings = true);

	bool removeSurfaceMaterial(grSurfaceMaterial* material);
	bool removeAllSurfaceMaterials();
};


#endif //SURFACE_MATERIAL_MANAGER_H