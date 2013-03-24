#ifndef MATERIAL_H
#define MATERIAL_H

#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/material/material_d3d8.h"
#endif //RENDER_D3D8

struct grMaterial:public grMaterialBase
{
	int         mRefCount;
	bool        mCanCache;

	std::string mName;

//functions
	grMaterial(grMaterialManager* materialManager):grMaterialBase(materialManager), mRefCount(1), mCanCache(true) {}

	grMaterial(const std::string& name, const color4& ambient, const color4& diffuse, const color4& emissive, 
		       const color4& specular, float power):grMaterialBase(NULL, ambient, diffuse, emissive, specular, power), 
			                                        mRefCount(1), mCanCache(true), mName(name) {}
	virtual ~grMaterial() {}
};

#endif //MATERIAL_H