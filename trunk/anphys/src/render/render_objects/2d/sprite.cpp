#include "sprite.h"

#include "render/render.h"
#include "render/render_objects/object_manager.h"
#include "render/texture_manager/texture.h"
#include "render/texture_manager/texture_manager.h"
#include "render_2d_object_mesh.h"


grSprite::grSprite( grRender* render, 
	                grTexture* texture          /*= NULL*/, 
	                const fRect& textureSrcRect /*= fRect(-1.0f, 0.0f, 0.0f, 0.0f)*/, 
	                const vec2& position        /*= vec2(0.0f, 0.0f)*/, 
					const vec2& size            /*= vec2(-1.0f, 0.0f)*/, 
					const vec2& scale           /*= vec2(1.0f, 1.0f)*/, 
					float angle                 /*= 0*/, 
					const vec2& rotationCenter  /*= vec2(0.0f, 0.0f)*/, 
					const color4& color         /*= color4(1.0f, 1.0f, 1.0f, 1.0f)*/ )
{
	//create mesh
	mMesh = new grRender2DObjectMesh(render, 4, 2);
	mMesh->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mMesh->mPolygonsBuffer[1] = poly3(0, 2, 3);
	mMesh->mVertexBuffer[0].z = 1.0f; mMesh->mVertexBuffer[1].z = 1.0f; 
	mMesh->mVertexBuffer[2].z = 1.0f; mMesh->mVertexBuffer[3].z = 1.0f;

	if (texture)
		mMesh->pushTexture(texture);
	
	if (textureSrcRect.leftTop.x < 0)
	{
		vec2 textureSize(0.0f, 0.0f);

		if (texture)
			textureSize = texture->mSize;

		mTextureSrcRect = fRect(vec2(0.0f, 0.0f), textureSize);
	}
	else 
	{
		mTextureSrcRect = textureSrcRect;
	}

	if (size.x < 0)
	{
		mSize = mTextureSrcRect.getSize();
	}
	else
	{
		mSize = size;
	}

	mPosition = position;
	mScale = scale;
	mAngle = angle;
	mRotationCenter = rotationCenter;

	setColor(color);

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
}

grSprite::grSprite( const grSprite& sprite )
{
	//create mesh
	mMesh = new grRender2DObjectMesh(sprite.mMesh->mRender, 4, 2);
	mMesh->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mMesh->mPolygonsBuffer[1] = poly3(0, 2, 3);

	for (unsigned int i = 0; i < 4; i++)
	{
		mMesh->mVertexBuffer[i] = sprite.mMesh->mVertexBuffer[i];
	}

	if (sprite.getTexture())
		mMesh->pushTexture(sprite.getTexture());

	mTextureSrcRect = sprite.mTextureSrcRect;
	mPosition = sprite.mPosition;
	mSize = sprite.mSize;
	mScale = sprite.mScale;
	mAngle = sprite.mAngle;
	mRotationCenter = sprite.mRotationCenter;


	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
}

grSprite::grSprite( grRender* render, cDataObject& dataObject )
{
	//create mesh
	mMesh = new grRender2DObjectMesh(render, 4, 2);
	mMesh->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mMesh->mPolygonsBuffer[1] = poly3(0, 2, 3);
	mMesh->mVertexBuffer[0].z = 1.0f; mMesh->mVertexBuffer[1].z = 1.0f; 
	mMesh->mVertexBuffer[2].z = 1.0f; mMesh->mVertexBuffer[3].z = 1.0f;

	serialize_(dataObject, AT_INPUT);
}

grSprite::grSprite(grRender* render, const std::string& file, const std::string& path)
{
	//create mesh
	mMesh = new grRender2DObjectMesh(render, 4, 2);
	mMesh->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mMesh->mPolygonsBuffer[1] = poly3(0, 2, 3);
	mMesh->mVertexBuffer[0].z = 1.0f; mMesh->mVertexBuffer[1].z = 1.0f; 
	mMesh->mVertexBuffer[2].z = 1.0f; mMesh->mVertexBuffer[3].z = 1.0f;

	cDataObject* spriteDataObject = 
		getDataObjectsManager().loadDataObject(file, cDataObjectsManager::DOT_XML)->getChild(path);

	serialize_(*spriteDataObject, AT_INPUT);
}

grSprite::~grSprite()
{
	safe_release(mMesh);
}

grSprite& grSprite::setPosition( const vec2& position )
{
	if (!(fabs(position.x - mPosition.x) > FLT_EPSILON || fabs(position.y - mPosition.y) > FLT_EPSILON))
		return *this;

	mPosition = position;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2 grSprite::getPosition() const
{
	return mPosition;
}

grSprite& grSprite::setScale( const vec2& scale )
{
	if (!(fabs(scale.x - mScale.x) > FLT_EPSILON || fabs(scale.y - mScale.y) > FLT_EPSILON))
		return *this;

	mScale = scale;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2 grSprite::getScale() const
{
	return mScale;
}

grSprite& grSprite::setAngle( float angle )
{
	if (fabs(angle - mAngle) < FLT_EPSILON)
		return *this;

	mAngle = angle;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

float grSprite::getAngle() const
{
	return mAngle;
}

grSprite& grSprite::setRotationCenter( const vec2& center )
{
	if (!(fabs(center.x - mRotationCenter.x) > FLT_EPSILON || fabs(center.y - mRotationCenter.y) > FLT_EPSILON))
		return *this;

	mRotationCenter = center;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2 grSprite::getRotationCenter() const
{
	return mRotationCenter;
}

grSprite& grSprite::setTextureSrcRect( const fRect& rect )
{
	mTextureSrcRect = rect;
	mNeedUpdateMeshTexCoords = true;
	return *this;
}

fRect grSprite::getTextureSrcRect() const
{
	return mTextureSrcRect;
}

grSprite* grSprite::clone() const
{
	return new grSprite(*this);
}

grSprite& grSprite::setTexture( grTexture* texture )
{
	mMesh->mTextures[0] = texture;
	return *this;
}

grTexture* grSprite::getTexture() const
{
	return mMesh->mTextures[0];
}

grSprite& grSprite::setSize( const vec2& size )
{
	if (!(fabs(size.x - mSize.x) > FLT_EPSILON || fabs(size.y - mSize.y) > FLT_EPSILON))
		return *this;

	mSize = size;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2 grSprite::getSize() const
{
	return mSize;
}

grSprite& grSprite::setColor( const color4& color, int vertexId /*= -1*/ )
{
	unsigned long ccolor = color.dwordARGB();

	if (vertexId < 0)
	{
		for (unsigned int i = 0; i < 4; i++)
			mMesh->mVertexBuffer[i].color = ccolor;
	}
	else
	{
		mMesh->mVertexBuffer[min(vertexId, 3)].color = ccolor;
	}

	return *this;
}

color4 grSprite::getColor( int vertexId /*= 0*/ )
{
	color4 res;
	res.setDwordARGB(mMesh->mVertexBuffer[max(min(vertexId, 4), 0)].color);
	return res;
}

void grSprite::draw()
{
	if (mNeedUpdateMeshVerticies) 
		updateMeshVerticies();
	if (mNeedUpdateMeshTexCoords)
		updateMeshTexCoords();

	mMesh->draw();
}

void grSprite::updateMeshVerticies()
{
	mNeedUpdateMeshVerticies = false;

	if (mAngle > FLT_EPSILON || mAngle < -FLT_EPSILON)
	{
		float radAngle = rad(mAngle);

		float sn = sinf(radAngle);
		float cs = cosf(radAngle);

		vec2 realSize = mSize.scale(mScale);

		vec2 xvec(cs, -sn);
		vec2 yvec(sn,  cs);

		vec2 offs = mPosition;

		if (fabs(mRotationCenter.x) > FLT_EPSILON || fabs(mRotationCenter.y) > FLT_EPSILON)
			offs = offs + xvec*(-mRotationCenter.x) - yvec*mRotationCenter.y;

		vec2 xsize = xvec*realSize.x;
		vec2 ysize = yvec*realSize.y;

		//vertex positions
		mMesh->mVertexBuffer[0].x = offs.x;                     mMesh->mVertexBuffer[0].y = offs.y;
		mMesh->mVertexBuffer[1].x = offs.x + xsize.x;           mMesh->mVertexBuffer[1].y = offs.y + xsize.y;
		mMesh->mVertexBuffer[2].x = offs.x + xsize.x + ysize.x; mMesh->mVertexBuffer[2].y = offs.y + xsize.y + ysize.y;
		mMesh->mVertexBuffer[3].x = offs.x + ysize.x;           mMesh->mVertexBuffer[3].y = offs.y + ysize.y;
	}
	else
	{
		vec2 realSize = mSize*mScale;

		mMesh->mVertexBuffer[0].x = mPosition.x;              mMesh->mVertexBuffer[0].y = mPosition.y;
		mMesh->mVertexBuffer[1].x = mPosition.x + realSize.x; mMesh->mVertexBuffer[1].y = mPosition.y;
		mMesh->mVertexBuffer[2].x = mPosition.x + realSize.x; mMesh->mVertexBuffer[2].y = mPosition.y + realSize.y;
		mMesh->mVertexBuffer[3].x = mPosition.x;              mMesh->mVertexBuffer[3].y = mPosition.y + realSize.y;
	}
}

void grSprite::updateMeshTexCoords()
{
	mNeedUpdateMeshTexCoords = false;

	if (mMesh->mTextures.size() == 0 || !mMesh->mTextures[0]) return;

	vec2 texSize = mMesh->mTextures[0]->mSize;
	vec2 invTexSize(1.0f/texSize.x, 1.0f/texSize.y);

	mMesh->mVertexBuffer[0].tu = mTextureSrcRect.leftTop.x*invTexSize.x;
	mMesh->mVertexBuffer[0].tv = mTextureSrcRect.leftTop.y*invTexSize.y;

	mMesh->mVertexBuffer[1].tu = mTextureSrcRect.rightDown.x*invTexSize.x;
	mMesh->mVertexBuffer[1].tv = mTextureSrcRect.leftTop.y*invTexSize.y;

	mMesh->mVertexBuffer[2].tu = mTextureSrcRect.rightDown.x*invTexSize.x;
	mMesh->mVertexBuffer[2].tv = mTextureSrcRect.rightDown.y*invTexSize.y;

	mMesh->mVertexBuffer[3].tu = mTextureSrcRect.leftTop.x*invTexSize.x;
	mMesh->mVertexBuffer[3].tv = mTextureSrcRect.rightDown.y*invTexSize.y;
}

serializeMethodImpl(grSprite)
{
	if (!serializeId(mPosition, "position"))
		mPosition = vec2(0);

	if (!serializeId(mScale, "scale"))
		mScale = vec2(1);

	if (!serializeId(mAngle, "angle"))
		mAngle = 0;

	if (!serializeId(mRotationCenter, "rotationCenter"))
		mRotationCenter = vec2(0);

	if (achieveType == AT_OUTPUT) 
	{
		if (mMesh->mTextures.size() > 0 && mMesh->mTextures[0])
		{
			serializeId(mMesh->mTextures[0]->mFileName, "texture");
			serializeId(mTextureSrcRect, "textureSrcRect");
		}
	}
	else
	{
		mMesh->removeAllTextures();

		std::string textureName;
		if (serializeId(textureName, "texture"))
		{
			grTexture* texture = mMesh->mRender->mTextures->createTexture(textureName);
			mMesh->pushTexture(texture);

			if (!serializeId(mTextureSrcRect, "textureSrcRect"))
				mTextureSrcRect = fRect(vec2(0), texture->mSize);
		}
	}

	if (!serializeId(mSize, "size"))
		mSize = mTextureSrcRect.getSize();

	if (achieveType == AT_INPUT)
	{
		color4 vertColors[4];

		if (!serializeArrId(vertColors, 4, "vertColors"))
		{
			color4 spriteColor;
			if (serializeId(spriteColor, "color"))
			{
				setColor(spriteColor);
			}
		}
		else
		{
			for (unsigned int i = 0; i < 4; i++)
				mMesh->mVertexBuffer[i].color = vertColors->dwordARGB();
		}
	}
	else
	{
		color4 vertColors[4];

		bool different = false;
		for (unsigned int i = 0; i < 4; i++)
		{
			vertColors[i].setDwordARGB( mMesh->mVertexBuffer[i].color );

			if (i > 0)
			{
				if (abs(vertColors[i].r - vertColors[i - 1].r) > 1 ||
					abs(vertColors[i].g - vertColors[i - 1].g) > 1 ||
					abs(vertColors[i].b - vertColors[i - 1].b) > 1 ||
					abs(vertColors[i].a - vertColors[i - 1].a) > 1)
				{
					different = true;
				}
			}
		}

		if (different)
			serializeArrId(vertColors, 4, "vertColors");
		else
			serializeId(vertColors[0], "color");
	}
	
	return true;
}