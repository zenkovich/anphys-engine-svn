#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H


#include <vector>
#include <string>

struct grMaterial;
struct grRender;
struct grRenderBaseInterface;

struct grMaterialManager
{
	grRender* mRender;

	typedef std::vector<grMaterial*> MaterialsList;
	MaterialsList mMaterials;

	grMaterialManager(grRenderBaseInterface* render);
	~grMaterialManager();

	grMaterial* addMaterial(grMaterial* material, bool canLoadMultiRef = true, bool willBeMultiRef = true);
	grMaterial* createMaterial(const std::string& materialFileName, bool canLoadMultiRef = true, bool willBeMultiRef = true) {}

	grMaterial* getMaterial(const std::string& name);

	bool removeMaterial(grMaterial* material);
	bool removeAllMaterials();
};


#endif //MATERIAL_MANAGER_H