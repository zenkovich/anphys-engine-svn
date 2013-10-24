#ifndef SURFACE_MATERIAL_D3D8_H
#define SURFACE_MATERIAL_D3D8_H


#include "render/surface/surface_material_base_interface.h"
#include "util/memory/mem_utils.h"
#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

struct grSurfaceMaterialBase:public grSurfaceMaterialBaseInterface
{
	grSurfaceMaterialBase(grSurfaceMaterialManager* surfaceMaterialManager):
		grSurfaceMaterialBaseInterface(surfaceMaterialManager) {  }

	void setup();

	const char* getStructRealization() { return "grSurfaceMaterialBaseD3d8"; }
};


#endif //SURFACE_MATERIAL_D3D8_H