#ifndef RENDER_2D_OBJECT_MESH_D3D8_H
#define RENDER_2D_OBJECT_MESH_D3D8_H

#include "render/render_objects/2d/render_2d_object_mesh_base_interface.h"

struct grRender2DObjectMeshBase:public grRender2DObjectMeshBaseInterface
{
	unsigned int mLastUsedVertexDataIndex;
	unsigned int mLastDrawingFrameIndex;

//functions
	grRender2DObjectMeshBase();
	grRender2DObjectMeshBase(unsigned int vertexCount, unsigned int polyCount);
	virtual ~grRender2DObjectMeshBase();

	virtual void draw();
};


#endif //RENDER_2D_OBJECT_MESH_D3D8_H