#include "sprite.h"

#include "render_system.h"
#include "texture.h"
#include "mesh.h"
#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

grSprite::grSprite( grTexture       texture        /*= NULL*/, 
	                const fRect&    textureSrcRect /*= fRect(-1.0f, 0.0f, 0.0f, 0.0f)*/, 
	                const vec2f&    position       /*= vec2f()*/, 
					const vec2f&    size           /*= vec2f(-1.0f, 0.0f)*/, 
					const vec2f&    scale          /*= vec2f(1.0f, 1.0f)*/, 
					float           angle          /*= 0*/, 
					const vec2f&    pivot          /*= vec2f()*/, 
					const color4&   color          /*= color4::white()*/ ):
	IRectDrawable(size, position, color, pivot)
{
	//create mesh
	mMesh = mnew grMesh(texture, 4, 2);

	mMesh->mIndexes[0] = 0; mMesh->mIndexes[1] = 1; mMesh->mIndexes[2] = 2;
	mMesh->mIndexes[3] = 0; mMesh->mIndexes[4] = 2; mMesh->mIndexes[5] = 3;

	mMesh->mVerticies[0].z = 1.0f; mMesh->mVerticies[1].z = 1.0f; 
	mMesh->mVerticies[2].z = 1.0f; mMesh->mVerticies[3].z = 1.0f;

	mMesh->mVertexCount = 4;
	mMesh->mPolyCount = 2;
	
	if (textureSrcRect.left < 0)
		mTextureSrcRect = fRect(vec2f(), texture.getSize());
	else
		mTextureSrcRect = textureSrcRect;

	if (size.x < 0)
		mSize = mTextureSrcRect.getSize();
	else
		mSize = size;

	for (int i = 0; i < 4; i++)
		mVertexColors[i] = color4::white();

	mScale = scale;
	mAngle = angle;

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
	mNeedUpdateMeshColors = true;

	initializeProperties();
}

grSprite::grSprite( const grSprite& sprite ):
	IRectDrawable(sprite)
{
	//create mesh
	mMesh = mnew grMesh(*sprite.mMesh);

	mTextureSrcRect = sprite.mTextureSrcRect;
	mScale = sprite.mScale;
	mAngle = sprite.mAngle;

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
	mNeedUpdateMeshColors = true;

	initializeProperties();
}

/*grSprite::grSprite( grRender* render, cDataObject& dataObject )
{
	//create mesh
	mMesh = new grRender2DObjectMesh(render, 4, 2);
	mMesh->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mMesh->mPolygonsBuffer[1] = poly3(0, 2, 3);
	mMesh->mVertexBuffer[0].z = 1.0f; mMesh->mVertexBuffer[1].z = 1.0f; 
	mMesh->mVertexBuffer[2].z = 1.0f; mMesh->mVertexBuffer[3].z = 1.0f;

	serialize_(dataObject, AT_INPUT);
}*/

grSprite::grSprite(pugi::xml_node& xmlNode)
{
	//create mesh
	mMesh = mnew grMesh(grTexture(), 4, 2);

	mMesh->mIndexes[0] = 0; mMesh->mIndexes[1] = 1; mMesh->mIndexes[2] = 2;
	mMesh->mIndexes[3] = 0; mMesh->mIndexes[4] = 2; mMesh->mIndexes[5] = 3;

	mMesh->mVerticies[0].z = 1.0f; mMesh->mVerticies[1].z = 1.0f; 
	mMesh->mVerticies[2].z = 1.0f; mMesh->mVerticies[3].z = 1.0f;

	mMesh->mVertexCount = 4;
	mMesh->mPolyCount = 2;

	serialize(xmlNode, cSerializeType::INPUT);

	initializeProperties();

	/*cDataObject* spriteDataObject = 
		getDataObjectsManager().loadDataObject(file, cDataObjectsManager::DOT_XML)->getChild(path);

	serialize_(*spriteDataObject, AT_INPUT);*/
}

grSprite::~grSprite()
{
	safe_release(mMesh);
}

shared<IRectDrawable> grSprite::clone() const
{
	return mnew grSprite(*this);
}

grSprite& grSprite::operator=(const grSprite& sprite)
{
	mMesh->setTexture(sprite.getTexture());
	mTextureSrcRect = sprite.mTextureSrcRect;
	mPosition = sprite.mPosition;
	mSize = sprite.mSize;
	mScale = sprite.mScale;
	mAngle = sprite.mAngle;
	mPivot = sprite.mPivot;

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
	mNeedUpdateMeshColors = true;

	return *this;
}

void grSprite::setScale( const vec2f& scale )
{
	if (equals(mScale, scale))
		return;

	mScale = scale;
	mNeedUpdateMeshVerticies = true;
}

vec2f grSprite::getScale() const
{
	return mScale;
}

void grSprite::setAngle( const float& angle )
{
	if (equals(angle, mAngle))
		return;

	mAngle = angle;
	mNeedUpdateMeshVerticies = true;
}

float grSprite::getAngle() const
{
	return mAngle;
}

void grSprite::setTextureSrcRect( const fRect& rect )
{
	if (equals(mTextureSrcRect, rect))
		return;

	mTextureSrcRect = rect;
	mNeedUpdateMeshTexCoords = true;
}

fRect grSprite::getTextureSrcRect() const
{
	return mTextureSrcRect;
}

void grSprite::setTexture( const grTexture& texture )
{
	mMesh->setTexture(texture);
}

grTexture grSprite::getTexture() const
{
	return mMesh->getTexture();
}

void grSprite::setVertexColor( const color4& color, int vertexId )
{
	mVertexColors[clamp(vertexId, 0, 3)] = color;
	mNeedUpdateMeshColors = true;
}

color4 grSprite::getVertexColor( int vertexId ) const
{
	return mVertexColors[clamp(vertexId, 0, 3)];
}

void grSprite::positionChanged()
{
	mNeedUpdateMeshVerticies = true;
}

void grSprite::sizeChanged()
{
	mNeedUpdateMeshVerticies = true;
}

void grSprite::pivotChanged()
{
	mNeedUpdateMeshVerticies = true;
}

void grSprite::colorChanged()
{
	mNeedUpdateMeshColors = true;
}

void grSprite::draw()
{
	if (mNeedUpdateMeshVerticies) 
		updateMeshVerticies();
	if (mNeedUpdateMeshTexCoords)
		updateMeshTexCoords();
	if (mNeedUpdateMeshColors)
		updateMeshColors();

	mMesh->draw();
}

void grSprite::updateMeshVerticies()
{
	mNeedUpdateMeshVerticies = false;

	if (!equals(mAngle, 0.0f))
	{
		float radAngle = deg2rad(mAngle);

		float sn = sinf(radAngle);
		float cs = cosf(radAngle);

		vec2f realSize = mSize.scale(mScale);

		vec2f xvec(cs,  sn);
		vec2f yvec(-sn, cs);

		vec2f offs = mPosition;
		vec2f piv = mPivot.scale(mScale);

		if (!equals(mPivot, vec2f()))
			offs = offs - xvec*piv.x - yvec*piv.y;

		vec2f xsize = xvec*realSize.x;
		vec2f ysize = yvec*realSize.y;

		//vertex positions
		mMesh->mVerticies[0].x = offs.x;                     mMesh->mVerticies[0].y = offs.y;
		mMesh->mVerticies[1].x = offs.x + xsize.x;           mMesh->mVerticies[1].y = offs.y + xsize.y;
		mMesh->mVerticies[2].x = offs.x + xsize.x + ysize.x; mMesh->mVerticies[2].y = offs.y + xsize.y + ysize.y;
		mMesh->mVerticies[3].x = offs.x + ysize.x;           mMesh->mVerticies[3].y = offs.y + ysize.y;
	}
	else
	{
		vec2f realSize = mSize.scale(mScale);
		vec2f pos = mPosition - mPivot.scale(mScale);

		mMesh->mVerticies[0].x = pos.x;              mMesh->mVerticies[0].y = pos.y;
		mMesh->mVerticies[1].x = pos.x + realSize.x; mMesh->mVerticies[1].y = pos.y;
		mMesh->mVerticies[2].x = pos.x + realSize.x; mMesh->mVerticies[2].y = pos.y + realSize.y;
		mMesh->mVerticies[3].x = pos.x;              mMesh->mVerticies[3].y = pos.y + realSize.y;
	}
}

void grSprite::updateMeshTexCoords()
{
	mNeedUpdateMeshTexCoords = false;

	vec2f texSize = mMesh->getTexture().getSize();
	if (equals(texSize, vec2f())) 
		return;

	vec2f invTexSize(1.0f/texSize.x, 1.0f/texSize.y);
	fRect tex = mTextureSrcRect*invTexSize;

	mMesh->mVerticies[0].tu = tex.left;  mMesh->mVerticies[0].tv = tex.top;
	mMesh->mVerticies[1].tu = tex.right; mMesh->mVerticies[1].tv = tex.top;
	mMesh->mVerticies[2].tu = tex.right; mMesh->mVerticies[2].tv = tex.down;
	mMesh->mVerticies[3].tu = tex.left;  mMesh->mVerticies[3].tv = tex.down;
}

void grSprite::updateMeshColors()
{
	for (int i = 0; i < 4; i++)
		mMesh->mVerticies[i].color = (mVertexColors[i]*mColor).dword();

	mNeedUpdateMeshColors = false;
}


SERIALIZE_METHOD_IMPL(grSprite)
{
	if (!SERIALIZE_ID(mPosition, "position"))
		mPosition = vec2f(0, 0);

	if (!SERIALIZE_ID(mScale, "scale"))
		mScale = vec2f(1, 1);

	if (!SERIALIZE_ID(mAngle, "angle"))
		mAngle = 0;

	if (!SERIALIZE_ID(mPivot, "rotationCenter"))
		mPivot = vec2f(0, 0);

	if (type == cSerializeType::OUTPUT) 
	{
		if (mMesh->mTexture.getFileName() != "")
		{
			string texFilename = mMesh->mTexture.getFileName();
			SERIALIZE_ID(texFilename, "texture");
			SERIALIZE_ID(mTextureSrcRect, "textureSrcRect");
		}
	}
	else
	{
		mMesh->setTexture(grTexture());

		string textureName;
		if (SERIALIZE_ID(textureName, "texture"))
		{
			grTexture texture = renderSystem()->getTextureFromFile(textureName);
			mMesh->setTexture(texture);

			if (!SERIALIZE_ID(mTextureSrcRect, "textureSrcRect"))
				mTextureSrcRect = fRect(vec2f(0, 0), texture.getSize());
		}
	}

	if (!SERIALIZE_ID(mSize, "size"))
		mSize = mTextureSrcRect.getSize();

	if (type == cSerializeType::INPUT)
	{
		color4 vertColors[4];

		if (!SERIALIZE_ARR_ID(vertColors, 4, "vertColors"))
		{
			color4 spriteColor;
			if (SERIALIZE_ID(spriteColor, "color"))
			{
				setColor(spriteColor);
			}
			else 
				setColor(color4::white());
		}
		else
		{
			for (unsigned int i = 0; i < 4; i++)
				mMesh->mVerticies[i].color = vertColors->dword();
		}
	}
	else
	{
		color4 vertColors[4];

		bool different = false;
		for (unsigned int i = 0; i < 4; i++)
		{
			vertColors[i].setDword( mMesh->mVerticies[i].color );

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
			SERIALIZE_ARR_ID(vertColors, 4, "vertColors");
		else
			SERIALIZE_ID(vertColors[0], "color");
	}
	
	return true;
}

void grSprite::initializeProperties()
{          
	REG_PROPERTY(grSprite, scale, setScale, getScale);
	REG_PROPERTY(grSprite, angle, setAngle, getAngle);
	REG_PROPERTY(grSprite, textureSrcRect, setTextureSrcRect, getTextureSrcRect);
	REG_PROPERTY(grSprite, texture, setTexture, getTexture);        
}

CLOSE_O2_NAMESPACE