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
	mMesh(NULL), mNeedUpdateMesh(true), mTransparency(1.0f), mNeedUpdateTransparency(false)
{
	if (parts > 0)
		createMesh(parts, texture);

	initializeProperties();
}

cStretchRect::cStretchRect( const cStretchRect& stretchRect )
{
	mMesh = mnew grMesh(*stretchRect.mMesh);

	mParts = stretchRect.mParts;
	mRect = stretchRect.mRect;
	mTransparency = stretchRect.mTransparency;

	mNeedUpdateMesh = true;
	mNeedUpdateTransparency = false;

	initializeProperties();
}

cStretchRect::cStretchRect(const grTexture& texture, int left, int top, int right, int bottom, 
	                       const fRect& texRect /*= fRect()*/, const color4& color /*= color4::white()*/):
	mNeedUpdateMesh(false), mTransparency(1.0f), mNeedUpdateTransparency(false)
{
	createMesh(9, texture);

	fRect rt = texRect;
	if (rt.isZero())
		rt = fRect(vec2f(), texture.getSize());
	
	float _left = (float)left;
	float _top = (float)top;
	float _right = (float)right;
	float _bottom = (float)bottom;

	mRect = fRect(0, 0, rt.getSizeX(), rt.getSizeY());

	// left top
	addPart(vec2f(0, 0), vec2f(0, 0), vec2f(0, 0), vec2f(_left, _top), 
		   fRect(rt.left, rt.top, rt.left + _left, rt.top + _top), false, color, color, color, color);	

	// top
	addPart(vec2f(0, 0), vec2f(_left, 0), vec2f(1, 0), vec2f(-_right, _top), 
		   fRect(rt.left + _left, rt.top, rt.right - _right, rt.top + _top), false, color, color, color, color);

	//right top
	addPart(vec2f(1, 0), vec2f(-_right, 0), vec2f(1, 0), vec2f(0, _top), 
		   fRect(rt.right - _right, rt.top, rt.right, rt.top + _top), false, color, color, color, color);
	

	//left
	addPart(vec2f(0, 0), vec2f(0, _top), vec2f(0, 1), vec2f(_left, -_bottom), 
		   fRect(rt.left, rt.top + _top, rt.left + _left, rt.down - _bottom), false, color, color, color, color);

	//center
	addPart(vec2f(0, 0), vec2f(_left, _top), vec2f(1, 1), vec2f(-_right, -_bottom), 
		   fRect(rt.left + _left, rt.top + _top, rt.right - _right, rt.down - _bottom), false, color, color, color, color);

	//right
	addPart(vec2f(1, 0), vec2f(-_right, _top), vec2f(1, 1), vec2f(0, -_bottom), 
		   fRect(rt.right - _right, rt.top + _top, rt.right, rt.down - _bottom), false, color, color, color, color);
	

	//left bottom
	addPart(vec2f(0, 1), vec2f(0, -_bottom), vec2f(0, 1), vec2f(_left, 0), 
		   fRect(rt.left, rt.down - _bottom, rt.left + _left, rt.down), false, color, color, color, color);

	//bottom
	addPart(vec2f(0, 1), vec2f(_left, -_bottom), vec2f(1, 1), vec2f(-_right, 0), 
		   fRect(rt.left + _left, rt.down - _bottom, rt.right - _right, rt.down), false, color, color, color, color);

	//right bottom
	addPart(vec2f(1, 1), vec2f(-_right, -_bottom), vec2f(1, 1), vec2f(0, 0), 
		   fRect(rt.right - _right, rt.down - _bottom, rt.right, rt.down), false, color, color, color, color);

	updateMesh();
	initializeProperties();
}

cStretchRect& cStretchRect::operator=( const cStretchRect& stretchRect )
{
	if (mMesh)
		safe_release(mMesh);

	mMesh = mnew grMesh(*stretchRect.mMesh);

	mParts = stretchRect.mParts;
	mRect = stretchRect.mRect;
	mTransparency = stretchRect.mTransparency;

	mNeedUpdateMesh = true;
	mNeedUpdateTransparency = false;

	return *this;
}

void cStretchRect::createMesh(int partsCount, const grTexture& texture)
{
	mMesh = mnew grMesh(texture, partsCount*4, partsCount*2);

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
	                       const fRect& texRect, bool wrapTexture /*= false*/, 
						   const color4& vertex0Color /*= color4::white()*/, 
						   const color4& vertex1Color /*= color4::white()*/, 
						   const color4& vertex2Color /*= color4::white()*/, 
						   const color4& vertex3Color /*= color4::white()*/ )
{
	mParts.push_back(Part(LTPercent, LTPixel, RBPercent, RBPixel, texRect, wrapTexture, vertex0Color, vertex1Color, vertex2Color,
		                  vertex3Color));
	mNeedUpdateMesh = true;

	return mParts.size() - 1;
}

void cStretchRect::removePart( int idx )
{
	if (idx < 0 || idx > (int)mParts.size() - 1)
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

fRect cStretchRect::getRect() const
{
	return mRect;
}

void cStretchRect::setPosition( const vec2f& pos )
{
	if (pos == mRect.getltCorner())
		return;
	
	vec2f diff = pos - mRect.getltCorner();

	mRect = mRect + diff;

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

	vec2f texSize = mMesh->getTexture().getSize();
	vec2f invTexSize(1.0f/texSize.x, 1.0f/texSize.y);

	vec2f rectSize = mRect.getSize();
	int i = 0;
	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it, i++)
	{
		vec2f ltPoint = mRect.getltCorner() + rectSize.scale(it->mLTPosPercent) + it->mLTPosPixel;
		vec2f rbPoint = mRect.getltCorner() + rectSize.scale(it->mRBPosPercent) + it->mRBPosPixel;

		color4 colors[4] = { it->mVertexColors[0], it->mVertexColors[1], it->mVertexColors[2], it->mVertexColors[3] };
		for(int j = 0; j < 4; j++)
			colors[j].a = (int)((float)colors[j].a*mTransparency);

		float ltex = it->mTextureSrcRect.left*invTexSize.x;
		float ttex = it->mTextureSrcRect.top*invTexSize.y;
		float rtex = it->mTextureSrcRect.right*invTexSize.x;
		float btex = it->mTextureSrcRect.down*invTexSize.y;

		if (it->mWrapTexture)
		{
			rtex *= (rbPoint.x - ltPoint.x)/it->mTextureSrcRect.getSizeX();
			btex *= (rbPoint.y - ltPoint.y)/it->mTextureSrcRect.getSizeY();
		}
		
		mMesh->mVerticies[i*4    ].set(ltPoint,                     1.0f, colors[0].dword(), ltex, ttex);
		mMesh->mVerticies[i*4 + 1].set(vec2f(rbPoint.x, ltPoint.y), 1.0f, colors[1].dword(), rtex, ttex);
		mMesh->mVerticies[i*4 + 2].set(rbPoint,                     1.0f, colors[2].dword(), rtex, btex);
		mMesh->mVerticies[i*4 + 3].set(vec2f(ltPoint.x, rbPoint.y), 1.0f, colors[3].dword(), ltex, btex);
	}

	mMesh->mVertexCount = mParts.size()*4;
	mMesh->mPolyCount = mParts.size()*2;

	mNeedUpdateMesh = false;
	mNeedUpdateTransparency = false;
}

void cStretchRect::draw(bool debug /*= false*/)
{
	if (mNeedUpdateMesh)
		updateMesh();

	if (mNeedUpdateTransparency)
		updateTransparency();

	if (mMesh)
		mMesh->draw();
	
	if (debug)
	{
		vec2f rectSize = mRect.getSize();
		FOREACH(PartsVec, mParts, prt) 
		{
			vec2f ltPoint = mRect.getltCorner() + rectSize.scale(prt->mLTPosPercent) + prt->mLTPosPixel;
			vec2f rbPoint = mRect.getltCorner() + rectSize.scale(prt->mRBPosPercent) + prt->mRBPosPixel;

			renderSystem()->drawRectFrame(ltPoint, rbPoint);
		}
	}
}

SERIALIZE_METHOD_IMPL(cStretchRect)
{
	return true;
}

void cStretchRect::initializeProperties()
{
	rect.init(this, &cStretchRect::setRect, &cStretchRect::getRect);
	position.init(this, &cStretchRect::setPosition, &cStretchRect::getPosition);
	size.init(this, &cStretchRect::setSize, &cStretchRect::getSize);
	transparency.initNonConstSetter(this, &cStretchRect::setTransparency, &cStretchRect::getTransparency);
}

void cStretchRect::setTransparency(float transparency)
{
	if (equals(mTransparency, transparency))
		return;

	mTransparency = transparency;
	mNeedUpdateTransparency = true;
}

float cStretchRect::getTransparency() const
{
	return mTransparency;
}

void cStretchRect::updateTransparency()
{
	int i = 0;
	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it, i++)
	{
		for(int j = 0; j < 4; j++) 
		{
			color4 cc = it->mVertexColors[j];			
			mMesh->mVerticies[i*4 + j].color = color4::dword(cc.r, cc.g, cc.b, (int)((float)cc.a*mTransparency));
		}
	}

	mNeedUpdateTransparency = false;
}

CLOSE_O2_NAMESPACE