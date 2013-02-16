#ifndef MESH_PART_H
#define MESH_PART_H

#include "../engine/engine_options.h"
#ifdef RENDER_D3D8
#include "d3d8/mesh_part_d3d8.h"
#endif

struct grMeshPart:public grMeshPartBase
{
	grMeshPart(grRender3DObjectMeshBase* mesh):grMeshPartBase(mesh) {}
};

#endif //MESH_PART_H