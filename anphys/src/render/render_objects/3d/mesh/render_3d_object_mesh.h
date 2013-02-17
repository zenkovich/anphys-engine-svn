#ifndef RENDER_3D_OBJECT_MESH_H
#define RENDER_3D_OBJECT_MESH_H


#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/objects/mesh/render_3d_object_mesh_d3d8.h"
#endif //RENDER_D3D8

struct grRender3DObjectMesh:public grRender3DObjectMeshBase
{
	grRender3DObjectMesh(grRenderObjectsManager* objectsManager, int maxVerticies, int maxPolygons):
			grRender3DObjectMeshBase(objectsManager, maxVerticies, maxPolygons) {}
};

#endif //RENDER_3D_OBJECT_MESH_H