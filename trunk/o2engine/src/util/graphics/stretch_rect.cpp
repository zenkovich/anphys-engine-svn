#include "stretch_rect.h"

#include "render_system/mesh.h"
#include "render_system/texture.h"
#include "render_system/render_system.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

cStretchRect::Part::Part()
{
}

cStretchRect::Part::Part( const vec2f& LTPercent, const vec2f& LTPixel, const vec2f& RBPercent, const vec2f& RBPixel, 
	                      const fRect& texRect, bool wrapTexture /*= false*/,
						  const color4& vertex0Color /*= color4::white()*/, 
						  const color4& vertex1Color /*= color4::white()*/, 
						  const color4& vertex2Color /*= color4::white()*/, 
						  const color4& vertex3Color /*= color4::white()*/ ):
	mLTPosPercent(LTPercent), mLTPosPixel(LTPixel), mRBPosPercent(RBPercent), mRBPosPixel(RBPixel), mTextureSrcRect(texRect),
	mWrapTexture(wrapTexture)
{
	mVertexColors[0] = vertex0Color; mVertexColors[1] = vertex1Color; 
	mVertexColors[2] = vertex2Color; mVertexColors[3] = vertex3Color;
}


cStretchRect::cStretchRect( int parts /*= 0*/, const grTexture& texture /*= grTexture()*/ ):
	mMesh(NULL), mNeedUpdateMesh(true), IRectDrawable(), mNeedUpdateColors(false)
{
	createMesh(max(parts, 1), texture);
}

cStretchRect::cStretchRect( const cStretchRect& stretchRect ):
	IRectDrawable(stretchRect)
{
	mMesh = mnew grMesh(*stretchRect.mMesh);

	mParts = stretchRect.mParts;

	mNeedUpdateMesh = true;
	mNeedUpdateColors = false;
}

cStretchRect::cStretchRect(const grTexture& texture, int left, int top, int right, int bottom, 
	                       const fRect& texRect /*= fRect()*/, const color4& color /*= color4::white()*/):
	mNeedUpdateMesh(false), IRectDrawable(), mNeedUpdateColors(false), mMesh(NULL)
{
	createMesh(9, texture);

	fRect rt = texRect;
	if (rt.isZero())
		rt = fRect(vec2f(), texture.getSize());
	
	float _left = (float)left;
	float _top = (float)top;
	float _right = (float)right;
	float _bottom = (float)bottom;

	mSize = rt.getSize();
	mColor = color;

	color4 wc = color4::white();

	// left top
	addPart(vec2f(0, 0), vec2f(0, 0), vec2f(0, 0), vec2f(_left, _top), 
		   fRect(rt.left, rt.top, rt.left + _left, rt.top + _top));	

	// top
	addPart(vec2f(0, 0), vec2f(_left, 0), vec2f(1, 0), vec2f(-_right, _top), 
		   fRect(rt.left + _left, rt.top, rt.right - _right, rt.top + _top));

	//right top
	addPart(vec2f(1, 0), vec2f(-_right, 0), vec2f(1, 0), vec2f(0, _top), 
		   fRect(rt.right - _right, rt.top, rt.right, rt.top + _top));
	

	//left
	addPart(vec2f(0, 0), vec2f(0, _top), vec2f(0, 1), vec2f(_left, -_bottom), 
		   fRect(rt.left, rt.top + _top, rt.left + _left, rt.down - _bottom));

	//center
	addPart(vec2f(0, 0), vec2f(_left, _top), vec2f(1, 1), vec2f(-_right, -_bottom), 
		   fRect(rt.left + _left, rt.top + _top, rt.right - _right, rt.down - _bottom));

	//right
	addPart(vec2f(1, 0), vec2f(-_right, _top), vec2f(1, 1), vec2f(0, -_bottom), 
		   fRect(rt.right - _right, rt.top + _top, rt.right, rt.down - _bottom));
	

	//left bottom
	addPart(vec2f(0, 1), vec2f(0, -_bottom), vec2f(0, 1), vec2f(_left, 0), 
		   fRect(rt.left, rt.down - _bottom, rt.left + _left, rt.down));

	//bottom
	addPart(vec2f(0, 1), vec2f(_left, -_bottom), vec2f(1, 1), vec2f(-_right, 0), 
		   fRect(rt.left + _left, rt.down - _bottom, rt.right - _right, rt.down));

	//right bottom
	addPart(vec2f(1, 1), vec2f(-_right, -_bottom), vec2f(1, 1), vec2f(0, 0), 
		   fRect(rt.right - _right, rt.down - _bottom, rt.right, rt.down));

	updateMesh();
}

cStretchRect& cStretchRect::operator=( const cStretchRect& stretchRect )
{
	safe_release(mMesh);
	mMesh = mnew grMesh(*stretchRect.mMesh);

	mParts = stretchRect.mParts;
	mPosition = stretchRect.mSize;
	mSize = stretchRect.mSize;
	mPivot = stretchRect.mPivot;
	mColor = stretchRect.mColor;

	mNeedUpdateMesh = true;
	mNeedUpdateColors = false;

	return *this;
}

shared<IRectDrawable> cStretchRect::clone() const
{
	return mnew cStretchRect(*this);
}

void cStretchRect::createMesh(int partsCount, const grTexture& texture)
{
	mMesh = mnew grMesh(texture, partsCount*4, partsCount*2);
	initMeshPolygons();
}

void cStretchRect::initMeshPolygons( int startIdx /*= 0*/ )
{
	int partsCount = mMesh->getMaxPolyCount()/2;
	for (int i = startIdx; i < partsCount; i++)
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
	                       const fRect& texRect, bool wrapTexture /*= false*/, 
						   const color4& vertex0Color /*= color4::white()*/, 
						   const color4& vertex1Color /*= color4::white()*/, 
						   const color4& vertex2Color /*= color4::white()*/, 
						   const color4& vertex3Color /*= color4::white()*/ )
{
	mParts.push_back(Part(LTPercent, LTPixel, RBPercent, RBPixel, texRect, wrapTexture, 
		                  vertex0Color, vertex1Color, vertex2Color, vertex3Color));

	int partsCount = mParts.size();

	if (mMesh->getMaxPolyCount() < mParts.size()*2)
	{
		mMesh->resize(partsCount*4, partsCount*2);
		initMeshPolygons(partsCount - 1);
	}

	mNeedUpdateMesh = true;

	return partsCount - 1;
}

void cStretchRect::removePart( int idx )
{
	if (idx < 0 || idx > (int)mParts.size() - 1)
		return;

	mParts.erase(mParts.begin() + idx);

	mNeedUpdateMesh = true;
}

void cStretchRect::positionChanged()
{
	mNeedUpdateMesh = true;
}

void cStretchRect::sizeChanged()
{
	mNeedUpdateMesh = true;
}

void cStretchRect::pivotChanged()
{
	mNeedUpdateMesh = true;
}

void cStretchRect::colorChanged()
{
	mNeedUpdateColors = true;
}

void cStretchRect::updateMesh()
{
	if (!mMesh)
		return;

	vec2f texSize = mMesh->getTexture().getSize();
	vec2f invTexSize(1.0f/texSize.x, 1.0f/texSize.y);

	vec2f pos = mPosition - mPivot;
	int i = 0;
	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it, i++)
	{
		vec2f ltPoint = pos + mSize.scale(it->mLTPosPercent) + it->mLTPosPixel;
		vec2f rbPoint = pos + mSize.scale(it->mRBPosPercent) + it->mRBPosPixel;

		fRect tex = it->mTextureSrcRect*invTexSize;

		if (it->mWrapTexture)
		{
			tex.right *= (rbPoint.x - ltPoint.x)/it->mTextureSrcRect.getSizeX();
			tex.down *= (rbPoint.y - ltPoint.y)/it->mTextureSrcRect.getSizeY();
		}
		
		mMesh->mVerticies[i*4    ].set(ltPoint,                     1.0f, (it->mVertexColors[0]*mColor).dword(), tex.left, tex.top);
		mMesh->mVerticies[i*4 + 1].set(vec2f(rbPoint.x, ltPoint.y), 1.0f, (it->mVertexColors[1]*mColor).dword(), tex.right, tex.top);
		mMesh->mVerticies[i*4 + 2].set(rbPoint,                     1.0f, (it->mVertexColors[2]*mColor).dword(), tex.right, tex.down);
		mMesh->mVerticies[i*4 + 3].set(vec2f(ltPoint.x, rbPoint.y), 1.0f, (it->mVertexColors[3]*mColor).dword(), tex.left, tex.down);
	}

	mMesh->mVertexCount = mParts.size()*4;
	mMesh->mPolyCount = mParts.size()*2;

	mNeedUpdateMesh = false;
	mNeedUpdateColors = false;
}

void cStretchRect::updateColors()
{
	int i = 0;
	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it, i++)
	{
		for(int j = 0; j < 4; j++) 
			mMesh->mVerticies[i*4 + j].color = (it->mVertexColors[j]*mColor).dword();
	}

	mNeedUpdateColors = false;
}

void cStretchRect::drawDebug()
{
	vec2f pos = mPosition - mPivot;
	int clr = 1;
	FOREACH(PartsVec, mParts, prt) 
	{
		vec2f ltPoint = pos + mSize.scale(prt->mLTPosPercent) + prt->mLTPosPixel;
		vec2f rbPoint = pos + mSize.scale(prt->mRBPosPercent) + prt->mRBPosPixel;

		renderSystem()->drawRectFrame(ltPoint, rbPoint, color4::someColor(clr++));
	}
}

void cStretchRect::draw()
{
	if (mNeedUpdateMesh)
		updateMesh();

	if (mNeedUpdateColors)
		updateColors();

	if (mMesh)
		mMesh->draw();
}

SERIALIZE_METHOD_IMPL(cStretchRect)
{
	return true;
}

CLOSE_O2_NAMESPACE