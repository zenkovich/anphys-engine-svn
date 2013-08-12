#ifndef MESH_H
#define MESH_H

#include "public.h"

#include "util/math/vertex.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

class grTexture;
class grRenderSystem;

class grMesh
{
	friend class grRenderSystem;
	friend class grSprite;

public:
	vertex2*        mVerticies;
	unsigned short* mIndexes;

	unsigned int    mVertexCount;
	unsigned int    mPolyCount;

protected:
	grRenderSystem* mRenderSystem;

	grTexture*      mTexture;

	unsigned int    mMaxVertexCount;
	unsigned int    mMaxPolyCount;

public:
	grMesh(grRenderSystem* renderSystem, grTexture* texture = NULL, unsigned int vertexCount = 4, 
		   unsigned int polyCount = 2);

	grMesh(const grMesh& mesh);

	~grMesh();

	void resize(unsigned int vertexCount, unsigned int polyCount);

	void draw();

	void setTexture(grTexture* texture);
	grTexture* getTexture() const;

	unsigned int getMaxVertexCount() const;
	unsigned int getMaxPolyCount() const;
};

CLOSE_O2_NAMESPACE

#endif //MESH_H