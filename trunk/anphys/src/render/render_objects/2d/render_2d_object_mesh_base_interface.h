#ifndef RENDER_2D_OBJECT_MESH_BASE_INTERFACE_H
#define RENDER_2D_OBJECT_MESH_BASE_INTERFACE_H

#include <vector>

#include "../render_object.h"
#include "util/other/type_intexation.h"
#include "util/math/mmath.h"

struct grTexture;
struct grRender;

struct grRender2DObjectMeshBaseInterface:public grRenderObject
{
	typedef std::vector<grTexture*> TexturesList;

	DEFINE_TYPE(grRender2DObjectMeshBaseInterface)

	vertex2d*    mVertexBuffer;
	poly3*       mPolygonsBuffer;
	unsigned int mVertexCount;
	unsigned int mPolygonsCount;
	bool         mDataUpdated;

	grRender*    mRender;

	TexturesList mTextures;

//functions
	grRender2DObjectMeshBaseInterface(grRender* render);
	grRender2DObjectMeshBaseInterface(grRender* render, unsigned int vertexCount, unsigned int polyCount);
	virtual ~grRender2DObjectMeshBaseInterface();

	virtual void draw() {}

	virtual void resize(unsigned int vertexCount, unsigned int polyCount);

	virtual void pushTexture(grTexture* texture);
	virtual void removeTexture(grTexture* texture);
	virtual void removeAllTextures();
};

#endif //RENDER_2D_OBJECT_MESH_BASE_INTERFACE_H