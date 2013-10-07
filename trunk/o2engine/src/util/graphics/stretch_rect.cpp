#include "stretch_rect.h"

#include "render_system/mesh.h"
#include "render_system/texture.h"

OPEN_O2_NAMESPACE

cStretchRect::Part::Part()
{
}

cStretchRect::Part::Part( const vec2f& LTPercent, const vec2f& LTPixel, const vec2f& RBPercent, const vec2f& RBPixel, 
	                      const fRect& texRect, const color4& vertex0Color /*= color4(255)*/, 
						  const color4& vertex1Color /*= color4(255)*/, const color4& vertex2Color /*= color4(255)*/, 
						  const color4& vertex3Color /*= color4(255)*/ ):
	mLTPosPercent(LTPercent), mLTPosPixel(LTPixel), mRBPosPercent(RBPercent), mRBPosPixel(RBPixel), mTextureSrcRect(texRect)
{
	mVertexColors[0] = vertex0Color; mVertexColors[1] = vertex1Color; 
	mVertexColors[2] = vertex2Color; mVertexColors[3] = vertex3Color;
}


cStretchRect::cStretchRect( grRenderSystem* renderSystem, int parts /*= 0*/, grTexture* texture /*= NULL*/ ):
	mMesh(NULL), mNeedUpdateMesh(true), mRenderSystem(renderSystem)
{
	if (parts > 0)
		createMesh(mRenderSystem, parts, texture);
}

cStretchRect::cStretchRect( const cStretchRect& stretchRect )
{
	mMesh = mnew grMesh(*stretchRect.mMesh);

	mParts = stretchRect.mParts;
	mRect = stretchRect.mRect;
	mRenderSystem = stretchRect.mRenderSystem;

	mNeedUpdateMesh = true;
}

cStretchRect& cStretchRect::operator=( const cStretchRect& stretchRect )
{
	if (mMesh)
		safe_release(mMesh);

	mMesh = mnew grMesh(*stretchRect.mMesh);

	mParts = stretchRect.mParts;
	mRect = stretchRect.mRect;
	mRenderSystem = stretchRect.mRenderSystem;

	mNeedUpdateMesh = true;

	return *this;
}

void cStretchRect::createMesh(grRenderSystem* renderSystem, int partsCount, grTexture* texture)
{
	mMesh = mnew grMesh(renderSystem, texture, partsCount*4, partsCount*2);

	for (int i = 0; i < partsCount; i++)
	{
		mMesh->mIndexes[i*2*3]     = i*4;
		mMesh->mIndexes[i*2*3 + 1] = i*4 + 1;
		mMesh->mIndexes[i*2*3 + 2] = i*4 + 2;
		mMesh->mIndexes[i*2*3 + 3] = i*4;
		mMesh->mIndexes[i*2*3 + 4] = i*4 + 2;
		mMesh->mIndexes[i*2*3 + 5] = i*4 + 3;
	}
}

int cStretchRect::addPart( const vec2f& LTPercent, const vec2f& LTPixel, const vec2f& RBPercent, const vec2f& RBPixel, 
	                       const fRect& texRect, const color4& vertex0Color /*= color4(255)*/, 
						   const color4& vertex1Color /*= color4(255)*/, const color4& vertex2Color /*= color4(255)*/, 
						   const color4& vertex3Color /*= color4(255)*/ )
{
	mParts.push_back(Part(LTPercent, LTPixel, RBPercent, RBPixel, texRect, vertex0Color, vertex1Color, vertex2Color,
		                  vertex3Color));
	mNeedUpdateMesh = true;
	return mParts.size() - 1;
}

void cStretchRect::removePart( int idx )
{
	if (idx < 0 || idx > mParts.size() - 1)
		return;

	mParts.erase(mParts.begin() + idx);

	mNeedUpdateMesh = true;
}

void cStretchRect::setRect( const fRect& rect )
{
	if (mRect == rect)
		return;

	mRect = rect;
	mNeedUpdateMesh = true;
}

const fRect& cStretchRect::getRect() const
{
	return mRect;
}

void cStretchRect::setPosition( const vec2f& pos )
{
	if (pos == mRect.getltCorner())
		return;
	
	mRect.left = pos.x;
	mRect.top = pos.y;

	mNeedUpdateMesh = true;
}

vec2f cStretchRect::getPosition() const
{
	return mRect.getltCorner();
}

void cStretchRect::setSize( const vec2f& size )
{
	if (size == mRect.getSize())
		return;
	
	mRect.right = mRect.left + size.x;
	mRect.down = mRect.top + size.y;

	mNeedUpdateMesh = true;
}

vec2f cStretchRect::getSize() const
{
	return mRect.getSize();
}

void cStretchRect::updateMesh()
{
	if (!mMesh)
		return;

	vec2f rectSize = mRect.getSize();
	int i = 0;
	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it, i++)
	{
		vec2f ltPoint = mRect.getltCorner() + rectSize.scale(it->mLTPosPercent) + it->mLTPosPixel;
		vec2f rbPoint = mRect.getltCorner() + rectSize.scale(it->mRBPosPercent) + it->mRBPosPixel;
		
		mMesh->mVerticies[i*4    ].set(ltPoint, it->mVertexColors[0].dword());
		mMesh->mVerticies[i*4 + 1].set(vec2f(rbPoint.x, ltPoint.y), it->mVertexColors[1].dword());
		mMesh->mVerticies[i*4 + 2].set(rbPoint, it->mVertexColors[2].dword());
		mMesh->mVerticies[i*4 + 3].set(vec2f(ltPoint.x, rbPoint.y), it->mVertexColors[3].dword());
	}

	mNeedUpdateMesh = false;
}

void cStretchRect::draw()
{
	if (mNeedUpdateMesh)
		updateMesh();

	if (mMesh)
		mMesh->draw();
}

SERIALIZE_METHOD_IMPL(cStretchRect)
{
	return true;
}

CLOSE_O2_NAMESPACE