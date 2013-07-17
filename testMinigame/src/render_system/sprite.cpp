#include "sprite.h"

#include "render_system.h"

Sprite::Sprite( RenderSystem* render, 
	            Texture* texture            /*= NULL*/, 
	            const fRect& textureSrcRect /*= fRect(-1.0f, 0.0f, 0.0f, 0.0f)*/, 
	            const vec2f& position       /*= vec2(0.0f, 0.0f)*/, 
				const vec2f& size           /*= vec2(-1.0f, 0.0f)*/, 
				const vec2f& scale          /*= vec2(1.0f, 1.0f)*/, 
				float angle                 /*= 0*/, 
				const vec2f& rotationCenter /*= vec2(0.0f, 0.0f)*/, 
				const color4& color         /*= color4(1.0f, 1.0f, 1.0f, 1.0f)*/ )
{
	mRenderSystem = render;

//create mesh
	mVerticies[0].z = 1.0f; mVerticies[1].z = 1.0f; 
	mVerticies[2].z = 1.0f; mVerticies[3].z = 1.0f;

	mTexture = texture;
	if (mTexture)
		mTexture->incRefCount();
	
//check left top parameter less zero
	if (textureSrcRect.leftTop.x < 0)
	{
		vec2f textureSize(0.0f, 0.0f);

		if (texture)
			textureSize = texture->getSize();

		//all texture as source
		mTextureSrcRect = fRect(vec2f(0.0f, 0.0f), textureSize);
	}
	else 
	{
		mTextureSrcRect = textureSrcRect;
	}

//check size
	if (size.x < 0)
	{
		mSize = mTextureSrcRect.getSize();
	}
	else
	{
		mSize = size;
	}

	mPosition       = position;
	mScale          = scale;
	mAngle          = angle;
	mRotationCenter = rotationCenter;

	setColor(color);

//need update verticies
	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
}

Sprite::Sprite( const Sprite& sprite )
{
	mRenderSystem = sprite.mRenderSystem;

//create mesh
	for (unsigned int i = 0; i < 4; i++)
	{
		mVerticies[i] = sprite.mVerticies[i];
	}

	mTexture = sprite.mTexture;
	if (mTexture)
		mTexture->incRefCount();

	mTextureSrcRect = sprite.mTextureSrcRect;
	mPosition       = sprite.mPosition;
	mSize           = sprite.mSize;
	mScale          = sprite.mScale;
	mAngle          = sprite.mAngle;
	mRotationCenter = sprite.mRotationCenter;

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
}

Sprite::~Sprite()
{
	mRenderSystem->releaseTexture(mTexture);
}

Sprite& Sprite::setPosition( const vec2f& position )
{
	if (position == mPosition)
		return *this;

	mPosition = position;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2f Sprite::getPosition() const
{
	return mPosition;
}

Sprite& Sprite::setScale( const vec2f& scale )
{
	if (mScale == scale)
		return *this;

	mScale = scale;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2f Sprite::getScale() const
{
	return mScale;
}

Sprite& Sprite::setAngle( float angle )
{
	if (fabs(angle - mAngle) < FLT_EPSILON)
		return *this;

	mAngle = angle;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

float Sprite::getAngle() const
{
	return mAngle;
}

Sprite& Sprite::setRotationCenter( const vec2f& center )
{
	if (mRotationCenter == center)
		return *this;

	mRotationCenter = center;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2f Sprite::getRotationCenter() const
{
	return mRotationCenter;
}

Sprite& Sprite::setTextureSrcRect( const fRect& rect )
{
	mTextureSrcRect = rect;
	mNeedUpdateMeshTexCoords = true;
	return *this;
}

fRect Sprite::getTextureSrcRect() const
{
	return mTextureSrcRect;
}

Sprite* Sprite::clone() const
{
	return new Sprite(*this);
}

Sprite& Sprite::setTexture( Texture* texture )
{
	if (mTexture)
		mTexture->decRefCount();

	mTexture = texture;

	if (mTexture)
		mTexture->incRefCount();

	return *this;
}

Texture* Sprite::getTexture() const
{
	return mTexture;
}

Sprite& Sprite::setSize( const vec2f& size )
{
	if (mSize == size)
		return *this;

	mSize = size;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2f Sprite::getSize() const
{
	return mSize;
}

Sprite& Sprite::setColor( const color4& color, int vertexId /*= -1*/ )
{
	unsigned long ccolor = color.dwordABGR();

	if (vertexId < 0)
	{
		for (unsigned int i = 0; i < 4; i++)
			mVerticies[i].color = ccolor;
	}
	else
	{
		mVerticies[min(vertexId, 3)].color = ccolor;
	}

	return *this;
}

color4 Sprite::getColor( int vertexId /*= 0*/ )
{
	color4 res;
	res.setDwordABGR(mVerticies[std::max<int>(std::min<int>(vertexId, 4), 0)].color);
	return res;
}

void Sprite::draw()
{
	if (mNeedUpdateMeshVerticies) 
		updateMeshVerticies();
	if (mNeedUpdateMeshTexCoords)
		updateMeshTexCoords();

	mRenderSystem->drawSprite(this);
}

void Sprite::updateMeshVerticies()
{
	mNeedUpdateMeshVerticies = false;

	if (mAngle > FLT_EPSILON || mAngle < -FLT_EPSILON)
	{
		float radAngle = mAngle/180.0f*3.1415926f;

		float sn = sinf(radAngle);
		float cs = cosf(radAngle);

		vec2f realSize = mSize.scale(mScale);

		vec2f xvec(cs, -sn);
		vec2f yvec(sn,  cs);

		vec2f offs = mPosition;

		if (fabs(mRotationCenter.x) > FLT_EPSILON || fabs(mRotationCenter.y) > FLT_EPSILON)
			offs = offs + xvec*(-mRotationCenter.x) - yvec*mRotationCenter.y;

		vec2f xsize = xvec*realSize.x;
		vec2f ysize = yvec*realSize.y;

		//vertex positions
		mVerticies[0].x = offs.x;                     mVerticies[0].y = offs.y;
		mVerticies[1].x = offs.x + xsize.x;           mVerticies[1].y = offs.y + xsize.y;
		mVerticies[2].x = offs.x + xsize.x + ysize.x; mVerticies[2].y = offs.y + xsize.y + ysize.y;
		mVerticies[3].x = offs.x + ysize.x;           mVerticies[3].y = offs.y + ysize.y;
	}
	else  //zero angle
	{
		vec2f realSize = mSize.scale(mScale);

		mVerticies[0].x = mPosition.x;              mVerticies[0].y = mPosition.y;
		mVerticies[1].x = mPosition.x + realSize.x; mVerticies[1].y = mPosition.y;
		mVerticies[2].x = mPosition.x + realSize.x; mVerticies[2].y = mPosition.y + realSize.y;
		mVerticies[3].x = mPosition.x;              mVerticies[3].y = mPosition.y + realSize.y;
	}
}

void Sprite::updateMeshTexCoords()
{
	mNeedUpdateMeshTexCoords = false;

	if (!mTexture) 
		return;

	vec2f texSize = mTexture->getSize();
	vec2f invTexSize(1.0f/texSize.x, 1.0f/texSize.y);

	mVerticies[0].tu = mTextureSrcRect.leftTop.x*invTexSize.x;
	mVerticies[0].tv = 1.0f - mTextureSrcRect.leftTop.y*invTexSize.y;

	mVerticies[1].tu = mTextureSrcRect.rightDown.x*invTexSize.x;
	mVerticies[1].tv = 1.0f - mTextureSrcRect.leftTop.y*invTexSize.y;

	mVerticies[2].tu = mTextureSrcRect.rightDown.x*invTexSize.x;
	mVerticies[2].tv = 1.0f - mTextureSrcRect.rightDown.y*invTexSize.y;

	mVerticies[3].tu = mTextureSrcRect.leftTop.x*invTexSize.x;
	mVerticies[3].tv = 1.0f - mTextureSrcRect.rightDown.y*invTexSize.y;
}

bool Sprite::load( const pugi::xml_node& xmlNode )
{
	pugi::xml_node textureFilenameNode = xmlNode.child("texture");
	if (!textureFilenameNode.empty())
	{		
		mTexture = mRenderSystem->createTexture(textureFilenameNode.attribute("filename").value());
	}

	if (mTexture)
	{
		pugi::xml_node texSrcNode = xmlNode.child("texSrc");
		if (texSrcNode.empty())
		{
			mTextureSrcRect = fRect(0, 0, mTexture->getSize().x, mTexture->getSize().y);
		}
		else
		{
			mTextureSrcRect.leftTop.x = texSrcNode.attribute("left").as_float();
			mTextureSrcRect.leftTop.y = texSrcNode.attribute("top").as_float();
			mTextureSrcRect.rightDown.x = texSrcNode.attribute("right").as_float();
			mTextureSrcRect.rightDown.y = texSrcNode.attribute("down").as_float();
		}
	}
	else
	{
		mTextureSrcRect = fRect(0, 0, 10, 10);
	}

	pugi::xml_node positionNode = xmlNode.child("position");
	if (positionNode.empty())
	{
		mPosition = vec2f(0, 0);
	}
	else
	{
		mPosition.x = positionNode.attribute("x").as_float();
		mPosition.y = positionNode.attribute("y").as_float();
	}

	pugi::xml_node sizeNode = xmlNode.child("size");
	if (sizeNode.empty())
	{
		mSize = mTextureSrcRect.getSize();
	}
	else
	{
		mSize.x = sizeNode.attribute("x").as_float();
		mSize.y = sizeNode.attribute("y").as_float();
	}

	pugi::xml_node scaleNode = xmlNode.child("scale");
	if (scaleNode.empty())
	{
		mScale = vec2f(1, 1);
	}
	else
	{
		mScale.x = scaleNode.attribute("x").as_float();
		mScale.y = scaleNode.attribute("y").as_float();
	}

	pugi::xml_node rotCenterNode = xmlNode.child("rotCenter");
	if (rotCenterNode.empty())
	{
		mRotationCenter = vec2f(0, 0);
	}
	else
	{
		mRotationCenter.x = rotCenterNode.attribute("x").as_float();
		mRotationCenter.y = rotCenterNode.attribute("y").as_float();
	}

	pugi::xml_node angleNode = xmlNode.child("rotation");
	if (angleNode.empty())
	{
		mAngle = 0;
	}
	else
	{
		mAngle = angleNode.attribute("angle").as_float();
	}

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;

	return true;
}
