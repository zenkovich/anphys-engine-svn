#ifndef SURFACE_MATERIAL_H
#define SURFACE_MATERIAL_H

#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/surface/surface_material_d3d8.h"
#endif //RENDER_D3D8

struct grSurfaceMaterial:public grSurfaceMaterialBase
{
	int mRefCount;
	bool mCanCache;

	std::string mName;

	grSurfaceMaterial(grSurfaceMaterialManager* materialManager = NULL):grSurfaceMaterialBase(materialManager), mRefCount(1), mCanCache(true) {}

	grSurfaceMaterial(const std::string& name, grSurfaceMaterialManager* materialManager = NULL):
		grSurfaceMaterialBase(materialManager), mRefCount(1), mCanCache(true), mName(name) {}

	/*grSurfaceMaterial(const std::string& name, const color4& ambient, const color4& diffuse, const color4& emissive, 
		       const color4& specular, float power):grMaterialBase(NULL, ambient, diffuse, emissive, specular, power), 
			                                        mRefCount(1), mCanCache(true), mName(name) {}*/
	virtual ~grSurfaceMaterial() {}
};

#endif //SURFACE_MATERIAL_H