#ifndef SURFACE_MATERIAL_D3D8_H
#define SURFACE_MATERIAL_D3D8_H


#include "../surface_material_base_interface.h"
#include "../../util/memory/mem_utils.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <d3dx8core.h>

struct grSurfaceMaterialBase:public grSurfaceMaterialBaseInterface
{
	grSurfaceMaterialBase(grSurfaceMaterialManager* surfaceMaterialManager):
		grSurfaceMaterialBaseInterface(surfaceMaterialManager) {  }

	void setup();

	std::string getStructRealization() { return "grSurfaceMaterialBaseD3d8"; }
};


#endif //SURFACE_MATERIAL_D3D8_H
