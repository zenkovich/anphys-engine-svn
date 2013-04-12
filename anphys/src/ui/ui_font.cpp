#include "ui_font.h"

#include "render/render.h"
#include "render/render_objects/2d/render_2d_object_mesh.h"
#include "render/texture_manager/texture.h"
#include "render/texture_manager/texture_manager.h"
#include "util/serialization/data_objects_manager.h"
#include "util/serialization/serialization.h"


uiFont::uiFont(grRender* render):mMesh(NULL), mRender(render)
{
	createMesh();
	mCharactedIdList.reserve(255);
	mNeedUpdateMesh = false;
}

uiFont::uiFont( const uiFont& font )
{
	createMesh();
	mMesh->pushTexture(font.mMesh->mTextures[0]);

	mCharacters      = font.mCharacters;
	mCharactedIdList = font.mCharactedIdList;

	mText            = font.mText;
	mTextArea        = font.mTextArea;
	mClippingArea    = font.mClippingArea;
	mClipping        = font.mClipping;
	mWordWrap        = font.mWordWrap;
	mHorAlign        = font.mHorAlign;
	mVerAlign        = font.mVerAlign;

	mNeedUpdateMesh = true;
}

uiFont::~uiFont()
{
	safe_release(mMesh);
}

void uiFont::load( const std::string& file, const std::string& objectPath )
{
	cDataObject* dataObject = getDataObjectsManager().loadDataObject(file, cDataObjectsManager::DOT_XML);
	load(dataObject);
}

void uiFont::load( cDataObject* dataObject )
{
	std::string textureFile;
	dataObject->getChildValue<std::string>("texture", textureFile);

	grTexture* texture = mRender->mTextures->createTexture(textureFile);
	mMesh->pushTexture(texture);

	vec2 texSize = texture->mSize;
	vec2 invTexSize(1.0f/texSize.x, 1.0f/texSize.y);

	cDataObject* symbolsList = dataObject->getChild("symbols");
	unsigned int characterIdx = 0;
	for (cDataObject::DataObjectsList::iterator it = symbolsList->mChilds.begin(); it != symbolsList->mChilds.end(); ++it)
	{
		if ((*it)->mId != "symbol") 
			continue;

		unsigned int charId;
		if (!(*it)->getChildValue("charId", charId))
			continue;

		fRect texSrcRect;
		if (!(*it)->getChildValue("texRect", texSrcRect))
			continue;

		texSrcRect.leftTop.scale(invTexSize);
		texSrcRect.rightDown.scale(invTexSize);

		mCharacters.push_back(texSrcRect);
		mCharactedIdList[charId] = characterIdx;

		characterIdx++;
	}

	mNeedUpdateMesh = false;
}

uiFont& uiFont::setText( const std::string& text )
{
	mText = text;
	mNeedUpdateMesh = true;
	return *this;
}

std::string uiFont::getText() const
{
	return mText;
}

uiFont& uiFont::setHorAlign( HorAlign align )
{
	mHorAlign = align;
	mNeedUpdateMesh = true;
	return *this;
}

uiFont::HorAlign uiFont::getHorAlign() const
{
	return mHorAlign;
}

uiFont& uiFont::setVerAlign( VerAlign align )
{
	mVerAlign = align;
	mNeedUpdateMesh = true;
	return *this;
}

uiFont::VerAlign uiFont::getVerAlign() const
{
	return mVerAlign;
}

uiFont& uiFont::setPosition( const vec2& position )
{
	mNeedUpdateMesh = true;
	return *this;
}

vec2 uiFont::getPosition()
{
	return mTextArea.leftTop;
}

uiFont& uiFont::setTextArea( const fRect& rect )
{
	mTextArea = rect;
	mNeedUpdateMesh = true;
	return *this;
}

fRect uiFont::getTextArea() const
{
	return mTextArea;
}

uiFont& uiFont::setClippingArea( const fRect& rect )
{
	mClippingArea = rect;
	mNeedUpdateMesh = true;
	return *this;
}

fRect uiFont::getClippingArea() const
{
	return mClippingArea;
}

uiFont& uiFont::setClipping( bool flag )
{
	mClipping = flag;
	mNeedUpdateMesh = true;
	return *this;
}

bool uiFont::isClipping() const
{
	return mClipping;
}

uiFont& uiFont::setDistCoef( const vec2& coef )
{
	mDistCoef = coef;
	mNeedUpdateMesh = true;
	return *this;
}

vec2 uiFont::getDistCoef() const
{
	return mDistCoef;
}

uiFont& uiFont::setScale( const vec2& scale )
{
	mScale = scale;
	mNeedUpdateMesh = true;
	return *this;
}

vec2 uiFont::getScale() const
{
	return mScale;
}

uiFont& uiFont::setColor( const color4& color )
{
	mColor = color;
	mNeedUpdateMesh = true;
	return *this;
}

color4 uiFont::getColor() const
{
	return mColor;
}


void uiFont::draw()
{
	if (mNeedUpdateMesh)
		updateMesh();

	mMesh->draw();
}

uiFont* uiFont::clone()
{
	return new uiFont(*this);
}

void uiFont::updateMesh()
{

}

void uiFont::createMesh()
{
	mMesh = new grRender2DObjectMesh(mRender, 4*nMaxSymbols, 2*nMaxSymbols);

	for (unsigned int i = 0; i < nMaxSymbols; i++)
	{
		unsigned int vertexId = i*4;
		mMesh->mPolygonsBuffer[i*2]     = poly3(vertexId, vertexId + 1, vertexId + 2);
		mMesh->mPolygonsBuffer[i*2 + 1] = poly3(vertexId, vertexId + 2, vertexId + 3);
	}
}