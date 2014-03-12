#ifndef MESH_H
#define MESH_H

#include "public.h"

#include "texture.h"
#include "util/objects.h"
#include "util/property.h"
#include "util/math/vertex.h"
#include "util/math/vector2.h"


OPEN_O2_NAMESPACE

class grRenderSystem;

/** Triangles mesh. Containing verticies, indeces of polygons, texture. */
class grMesh: public IDrawable
{
	friend class grRenderSystem;
	friend class grSprite;

public:
	vertex2*  mVerticies;      /**< Vertex buffer. */
	uint16*   mIndexes;        /**< Index buffer. */

	uint32    mVertexCount;    /**< Current verticies count. */
	uint32    mPolyCount;      /**< Current polygons in mesh. */

protected:
	grTexture mTexture;        /**< Texture. */

	uint32    mMaxVertexCount; /**< Max size of vertex buffer. */
	uint32    mMaxPolyCount;   /**< Max polygons count, mMaxPolyCount*3 - os index buffer max size. */

public:
	//properties
	PROPERTY(grMesh, grTexture) texture;     /** Texture property, uses set/getTexture. */
	PROPERTY(grMesh, uint32) maxVertexCount; /** Max vertex count property, uses set/getMaxVertexCount. */
	PROPERTY(grMesh, uint32) maxPolyCount;   /** Max polygons count property, uses set/getMaxPolyCount. */

	/** ctor. */
	grMesh(grTexture texture = grTexture(), uint32 vertexCount = 4, uint32 polyCount = 2);

	/** copy ctor. */
	grMesh(const grMesh& mesh);

	/** dtor. */
	~grMesh();

	/** Resising mesh buffers, losts data. */
	void resize(unsigned int vertexCount, unsigned int polyCount);

	/** Drawing mesh. */
	void draw();

	/** Sets texture. */
	void setTexture(const grTexture& texture);

	/** Returns texture ptr. */
	grTexture getTexture() const;

	/** Sets max vertex count buffer. */
	void setMaxVertexCount(const uint32& count);

	/** Sets max polygons count buffer. */
	void setMaxPolyCount(const uint32& count);

	/** Returns max vertex buffer size. */
	uint32 getMaxVertexCount() const;

	/** Returns max polygons count. */
	uint32 getMaxPolyCount() const;

protected:
	/** Initializing properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif //MESH_H