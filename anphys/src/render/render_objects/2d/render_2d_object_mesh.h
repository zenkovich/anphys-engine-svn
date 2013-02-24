#ifndef RENDER_2D_OBJECT_MESH_H
#define RENDER_2D_OBJECT_MESH_H

#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/objects/2d/render_2d_object_mesh_d3d8.h"
#endif //RENDER_D3D8

struct grRender2DObjectMesh:grRender2DObjectMeshBase
{
	grRender2DObjectMesh():grRender2DObjectMeshBase() {}
	grRender2DObjectMesh(unsigned int vertexCount, unsigned int polyCount):grRender2DObjectMeshBase(vertexCount, polyCount) {}
};

#endif //RENDER_2D_OBJECT_MESH_H