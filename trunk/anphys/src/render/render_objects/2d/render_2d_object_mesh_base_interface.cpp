#include "render_2d_object_mesh_base_interface.h"

#include <algorithm>

//utils
#include "util/memory/mem_utils.h"

//render
#include "render/render.h"
#include "render/render_objects/object_manager.h"
#include "render/texture_manager/texture.h"
#include "render/texture_manager/texture_manager.h"

REGIST_TYPE(grRender2DObjectMeshBaseInterface)

grRender2DObjectMeshBaseInterface::grRender2DObjectMeshBaseInterface(grRender* render)
	:mVertexBuffer(NULL), mVertexCount(0), mPolygonsBuffer(NULL), mPolygonsCount(0), mDataUpdated(true), mRender(render)
{
}

grRender2DObjectMeshBaseInterface::grRender2DObjectMeshBaseInterface( grRender* render, unsigned int vertexCount, 
	                                                                  unsigned int polyCount )
	:mVertexBuffer(NULL), mVertexCount(0), mPolygonsBuffer(NULL), mPolygonsCount(0), mDataUpdated(true), mRender(render)
{
	resize(vertexCount, polyCount);
}

grRender2DObjectMeshBaseInterface::~grRender2DObjectMeshBaseInterface()
{
	safe_release_arr(mVertexBuffer);
	safe_release_arr(mPolygonsBuffer);
}

void grRender2DObjectMeshBaseInterface::resize( unsigned int vertexCount, unsigned int polyCount )
{
	safe_release_arr(mVertexBuffer);
	safe_release_arr(mPolygonsBuffer);

	mVertexBuffer = new vertex2d[vertexCount];
	mVertexCount = vertexCount;

	mPolygonsBuffer = new poly3[polyCount];
	mPolygonsCount = polyCount;

	mDataUpdated = true;
}

void grRender2DObjectMeshBaseInterface::pushTexture( grTexture* texture )
{
	if (!texture) return;

	texture->mRefCount++;
	mTextures.push_back(texture);

	mDataUpdated = true;
}

void grRender2DObjectMeshBaseInterface::removeTexture( grTexture* texture )
{
	TexturesList::iterator fnd = std::find(mTextures.begin(), mTextures.end(), texture);

	if (fnd == mTextures.end()) return;
	mTextures.erase(fnd);

	mRender->mTextures->removeTexture(texture);

	mDataUpdated = true;
}

void grRender2DObjectMeshBaseInterface::removeAllTextures()
{
	for (TexturesList::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		mRender->mTextures->removeTexture(*it);
	}

	mTextures.clear();

	mDataUpdated = true;
}
