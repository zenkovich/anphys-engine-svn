#include "mesh.h"

#include "render_system.h"
#include "texture.h"

OPEN_O2_NAMESPACE

grMesh::grMesh( grTexture texture /*= grTexture()*/, uint32 vertexCount /*= 4*/, uint32 polyCount /*= 2*/ ):
	mVerticies(NULL), mIndexes(NULL), mMaxPolyCount(0), mMaxVertexCount(0), mVertexCount(0), mPolyCount(0)
{
	setTexture(texture);
	resize(vertexCount, polyCount);
	initializeProperties();
}

grMesh::grMesh( const grMesh& mesh ):
	mVerticies(NULL), mIndexes(NULL), mMaxVertexCount(0), mMaxPolyCount(0)
{
	setTexture(mesh.mTexture);

	resize(mesh.mMaxVertexCount, mesh.mMaxPolyCount);

	mVertexCount = mesh.mVertexCount;
	mPolyCount = mesh.mPolyCount;

	memcpy(mVerticies, mesh.mVerticies, mesh.mMaxVertexCount*sizeof(vertex2));
	memcpy(mIndexes, mesh.mIndexes, mesh.mMaxPolyCount*3*sizeof(uint16));

	initializeProperties();
}

grMesh::~grMesh()
{
	safe_release_arr(mVerticies);
	safe_release_arr(mIndexes);
}

void grMesh::setTexture( const grTexture& texture )
{
	mTexture = texture;
}

grTexture grMesh::getTexture() const
{
	return mTexture;
}

void grMesh::resize( uint32 vertexCount, uint32 polyCount )
{
	safe_release_arr(mVerticies);
	safe_release_arr(mIndexes);

	mVerticies = mnew vertex2[vertexCount];
	mIndexes = mnew uint16[polyCount*3];

	mMaxVertexCount = vertexCount;
	mMaxPolyCount = polyCount;

	mVertexCount = 0;
	mPolyCount = 0;
}

void grMesh::draw()
{
	renderSystem()->drawMesh( this );
}

uint32 grMesh::getMaxVertexCount() const
{
	return mMaxVertexCount;
}

uint32 grMesh::getMaxPolyCount() const
{
	return mMaxPolyCount;
}

void grMesh::setMaxVertexCount( const uint32& count )
{
	safe_release_arr(mVerticies);
	mVerticies = mnew vertex2[count];
	mMaxVertexCount = count;
	mVertexCount = 0;
}

void grMesh::setMaxPolyCount( const uint32& count )
{
	safe_release_arr(mIndexes);
	mIndexes = mnew uint16[count*3];
	mMaxPolyCount = count;
	mPolyCount = 0;
}

void grMesh::initializeProperties()
{
	REG_PROPERTY(grMesh, texture, setTexture, getTexture);
	REG_PROPERTY(grMesh, maxVertexCount, setMaxVertexCount, getMaxVertexCount);
	REG_PROPERTY(grMesh, maxPolyCount, setMaxPolyCount, getMaxPolyCount);
}

CLOSE_O2_NAMESPACE