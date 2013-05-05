#include "ui_font.h"

#include "render/render.h"
#include "render/render_objects/2d/render_2d_object_mesh.h"
#include "render/render_state/2d_render_state.h"
#include "render/texture_manager/texture.h"
#include "render/texture_manager/texture_manager.h"
#include "util/serialization/data_objects_manager.h"
#include "util/serialization/serialization.h"

#include "util/file_system/file_system.h"
#include "util/file_system/file.h"


uiFont::uiFont(grRender* render):mMesh(NULL), mRender(render)
{
	createMesh();
	for (unsigned int i = 0; i < nMaxCharId; i++)
		mCharactedIdList[i] = 0;
	mNeedUpdateMesh = false;

	mTextArea = fRect(0, 0, 0, 0);
	mClippingArea = fRect(0, 0, 0, 0);
	mClipping = false;
	mWordWrap = true;
	mHorAlign = AL_CENTER;
	mVerAlign = AL_MIDDLE;
	mDistCoef = vec2(0, 0);
	mColor = color4(1.0f, 1.0f, 1.0f, 1.0f);
	mScale = vec2(1, 1);
}

uiFont::uiFont( const uiFont& font )
{
	mRender = font.mRender;

	createMesh();
	mMesh->pushTexture(font.mMesh->mTextures[0]);

	mCharacters      = font.mCharacters;
	for (unsigned int i = 0; i < nMaxCharId; i++)
		mCharactedIdList[i] = font.mCharactedIdList[i];

	mText            = font.mText;
	mTextArea        = font.mTextArea;
	mClippingArea    = font.mClippingArea;
	mClipping        = font.mClipping;
	mWordWrap        = font.mWordWrap;
	mHorAlign        = font.mHorAlign;
	mVerAlign        = font.mVerAlign;
	mDistCoef        = font.mDistCoef;
	mScale           = font.mScale;
	mColor           = font.mColor;

	mNeedUpdateMesh = true;
}

uiFont::~uiFont()
{
	safe_release(mMesh);
}

void uiFont::load( const std::string& file, const std::string& objectPath )
{
	cDataObject* dataObject = getDataObjectsManager().loadDataObject(file, cDataObjectsManager::DOT_XML);
	load(dataObject->getChild(objectPath));
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

		/*texSrcRect.leftTop.scale(invTexSize);
		texSrcRect.rightDown.scale(invTexSize);*/

		mCharacters.push_back(texSrcRect);
		mCharactedIdList[charId] = characterIdx;

		characterIdx++;
	}

	mNeedUpdateMesh = false;
}

uiFont& uiFont::setText( const std::string& text )
{
	mText = convertStringToWide(text);
	mNeedUpdateMesh = true;
	return *this;
}

std::string uiFont::getText()
{
	return convertWideToString(mText);
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
	vec2 size = mTextArea.getSize();
	mTextArea.leftTop = position;
	mTextArea.rightDown = position + size;

	/*vec2 center;
	if (mHorAlign == AL_LEFT)
		center.x = mTextArea.leftTop.x;
	else if (mHorAlign == AL_CENTER)
		center.x = (mTextArea.leftTop.x + mTextArea.rightDown.x)*0.5f;
	else if (mHorAlign == AL_RIGHT)
		center.y = mTextArea.rightDown.x;

	if (mVerAlign == AL_TOP)
		center.y = mTextArea.leftTop.y;
	else if (mVerAlign == AL_MIDDLE)
		center.y = (mTextArea.leftTop.y + mTextArea.rightDown.y)*0.5f;
	else if (mVerAlign == AL_BOTTOM)
		center.y = mTextArea.rightDown.y;

	vec2 diff = position - center;

	mTextArea.plusVector(diff);*/

	mNeedUpdateMesh = true;
	return *this;
}

vec2 uiFont::getPosition()
{
	/*vec2 center;
	if (mHorAlign == AL_LEFT)
		center.x = mTextArea.leftTop.x;
	else if (mHorAlign == AL_CENTER)
		center.x = (mTextArea.leftTop.x + mTextArea.rightDown.x)*0.5f;
	else if (mHorAlign == AL_RIGHT)
		center.y = mTextArea.rightDown.x;

	if (mVerAlign == AL_TOP)
		center.y = mTextArea.leftTop.y;
	else if (mVerAlign == AL_MIDDLE)
		center.y = (mTextArea.leftTop.y + mTextArea.rightDown.y)*0.5f;
	else if (mVerAlign == AL_BOTTOM)
		center.y = mTextArea.rightDown.y;

	return center;*/

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

	gr2DRenderState* renderState = static_cast<gr2DRenderState*>(mRender->getCurrentRenderState());

	for (CacheLinesList::iterator it = mCahcedLines.begin(); it != mCahcedLines.end(); ++it)
	{
		/*for (RectsList::iterator jt = it->mCharactersGeometry.begin(); jt != it->mCharactersGeometry.end(); ++jt)
		{
			renderState->pushRect(jt->leftTop, jt->rightDown);
		}*/

		renderState->pushRect(it->mRect.leftTop, it->mRect.rightDown);
	}
}

uiFont* uiFont::clone()
{
	return new uiFont(*this);
}

void clipRect(const fRect& clippingRect, vertex2d* verticies)
{

}

void uiFont::updateMesh()
{
	mCahcedLines.clear();

	if (mText.length() == 0)
	{
		mMesh->mVertexCount = 0;
		mMesh->mPolygonsCount = 0;
		mNeedUpdateMesh = false;
		return;
	}

	std::vector<vec2> lineSize;

	float currentLineWidth = 0;
	float currentLineHeight = 0;
	float totalLinesHeight = 0;
	float totalLinesWidth = 0;

	unsigned int lastDelimer = 0;

	unsigned int length = mText.length();
	for (unsigned int i = 0; i < length + 1; i++)
	{
		char16_t symbol = mText[min(i, length - 1)];


		bool breakSymbol = false;
		if (symbol == '\n' || i == length)

		/*if (symbol == '\n' || i == length)
		{
			totalLinesWidth = max(totalLinesWidth, currentLineWidth);
			totalLinesHeight += currentLineHeight;

			lineSize.push_back(vec2(currentLineWidth, currentLineHeight));
			currentLineWidth = 0;

			continue;
		}*/

		if (symbol > nMaxCharId)
			continue;

		unsigned int symbolId = mCharactedIdList[symbol];
		vec2 symbolSize = mCharacters[symbolId].getSize().scale(mScale);

		currentLineHeight = max(symbolSize.y + mDistCoef.y, currentLineHeight);
		currentLineWidth += symbolSize.x + mDistCoef.x;
	}

	float lineYPos = 0;
	if (mVerAlign == AL_TOP)
		lineYPos = mTextArea.leftTop.y + lineSize[0].y;
	else if (mVerAlign == AL_MIDDLE)
		lineYPos = (mTextArea.leftTop.y + mTextArea.rightDown.y)*0.5f - totalLinesHeight*0.5f +  + lineSize[0].y;
	else if (mVerAlign == AL_BOTTOM)
		lineYPos = mTextArea.rightDown.y - totalLinesHeight + lineSize[0].y;

	vec2 invTexSize(1.0f/mMesh->mTextures[0]->mSize.x, 1.0f/mMesh->mTextures[0]->mSize.y);

	float lineXPos = 0;
	int lineIdx = -1;
	vec2 currentLineSize;
	unsigned long color = mColor.dwordARGB();
	mMesh->mVertexCount = 0;
	mMesh->mPolygonsCount = 0;
	for (unsigned int i = 0; i < length + 1; i++)
	{
		char16_t symbol = mText[min(i, length - 1)];

		if (symbol == '\n' || i == length || i == 0)
		{
			lineIdx++;

			if (i < length - 1)
			{
				currentLineSize = lineSize[lineIdx];
			}

			if (mHorAlign == AL_LEFT)
				lineXPos = mTextArea.leftTop.x;
			else if (mHorAlign == AL_CENTER)
				lineXPos = (mTextArea.leftTop.x + mTextArea.rightDown.x)*0.5f - currentLineSize.x*0.5f;
			else if (mHorAlign == AL_RIGHT)
				lineXPos = mTextArea.rightDown.x - currentLineSize.x;

			if (i < length - 1 || i == 0)
			{
				mCahcedLines.push_back(StrLineCache());

				mCahcedLines.back().mStartSymbol = i;
				if (i != 0) 
					mCahcedLines.back().mStartSymbol += 1;
				mCahcedLines.back().mEndSymbol = mCahcedLines.back().mStartSymbol;

				mCahcedLines.back().mRect.leftTop = vec2(lineXPos, lineYPos);
				mCahcedLines.back().mRect.rightDown = vec2(lineXPos, lineYPos);
			}

			if (i != 0)
			{
				lineYPos += currentLineSize.y + mDistCoef.y;
				continue;
			}
		}

		if (symbol > nMaxCharId)
			continue;

		unsigned int symbolId = mCharactedIdList[symbol];
		fRect symbolRect = mCharacters[symbolId];
		vec2 symbolSize = symbolRect.getSize().scale(mScale);

		fRect symbolMeshRect(vec2(lineXPos, lineYPos - symbolSize.y), vec2(lineXPos + symbolSize.x, lineYPos));

		mMesh->mVertexBuffer[mMesh->mVertexCount++] = 
			vertex2d(symbolMeshRect.leftTop.x, symbolMeshRect.leftTop.y, 1.0f, 
			symbolRect.leftTop.x*invTexSize.x, symbolRect.leftTop.y*invTexSize.y, color);

		mMesh->mVertexBuffer[mMesh->mVertexCount++] = 
			vertex2d(symbolMeshRect.rightDown.x, symbolMeshRect.leftTop.y, 1.0f, 
			symbolRect.rightDown.x*invTexSize.x, symbolRect.leftTop.y*invTexSize.y, color);

		mMesh->mVertexBuffer[mMesh->mVertexCount++] = 
			vertex2d(symbolMeshRect.rightDown.x, symbolMeshRect.rightDown.y, 1.0f, 
			symbolRect.rightDown.x*invTexSize.x, symbolRect.rightDown.y*invTexSize.y, color);

		mMesh->mVertexBuffer[mMesh->mVertexCount++] = 
			vertex2d(symbolMeshRect.leftTop.x, symbolMeshRect.rightDown.y, 1.0f, 
			symbolRect.leftTop.x*invTexSize.x, symbolRect.rightDown.y*invTexSize.y, color);

		if (lineYPos - symbolSize.y < mCahcedLines.back().mRect.leftTop.y)
			mCahcedLines.back().mRect.leftTop.y = symbolMeshRect.leftTop.y;

		mCahcedLines.back().mCharactersGeometry.push_back(symbolMeshRect);
		mCahcedLines.back().mEndSymbol = i;

		mMesh->mPolygonsCount += 2;

		lineXPos += symbolSize.x + mDistCoef.x;
	}

	fRect spaceSymbolRect = mCharacters[mCharactedIdList[' ']];

	for (CacheLinesList::iterator it = mCahcedLines.begin(); it != mCahcedLines.end(); it++)
	{
		vec2 lastCharacterLeftTop = it->mRect.getltCorner();

		if (it->mCharactersGeometry.size() > 0)
			lastCharacterLeftTop = it->mCharactersGeometry.back().getrtCorner();

		it->mCharactersGeometry.push_back(fRect(lastCharacterLeftTop, lastCharacterLeftTop + spaceSymbolRect.getSize()));

		it->mRect.rightDown = it->mCharactersGeometry.back().rightDown;
		it->mEndSymbol++;
	}

	if (mMesh->mVertexCount > 0)
	{
		mRealTextRect = fRect(mMesh->mVertexBuffer[0].x, mMesh->mVertexBuffer[0].y, 
			                  mMesh->mVertexBuffer[0].x, mMesh->mVertexBuffer[0].y);

		for (unsigned int i = 0; i < mMesh->mVertexCount; i++)
		{
			vertex2d* vertex = &mMesh->mVertexBuffer[i];

			mRealTextRect.leftTop.x = min(mRealTextRect.leftTop.x, vertex->x);
			mRealTextRect.leftTop.y = min(mRealTextRect.leftTop.y, vertex->y);

			mRealTextRect.rightDown.x = max(mRealTextRect.rightDown.x, vertex->x);
			mRealTextRect.rightDown.y = max(mRealTextRect.rightDown.y, vertex->y);
		}
	}
	else
	{
		mRealTextRect = fRect(0, 0, 0, 0);
	}

	mNeedUpdateMesh = false;
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

void uiFont::loadWelloreFormat( const std::string& filename )
{
	cFile* file = getFileSystem().openFileRead(filename);

	char* data = new char[file->getDataSize()];
	file->readFullData(data);

	std::string str = data;

	safe_release_arr(data);

	getFileSystem().closeFile(file);

	unsigned int caret = 0;
	unsigned int ccaret = 0;

	cDataObject dataObj;
	cDataObject* child = dataObj.addChild("font");

	ccaret = str.find("\"");
	caret = ccaret;
	ccaret = str.find("\"", caret + 1);

	std::string textureStr = str.substr(caret + 1, ccaret - caret - 1);

	child->addChild(textureStr, "texture");

	cDataObject* symbolsChild = child->addChild("symbols");

	caret = ccaret;
	ccaret = str.find("{", caret); caret = ccaret;

	while(true)
	{
		ccaret = str.find("{", caret + 1); caret = ccaret;
		if (caret ==  str.npos) break;
		ccaret = str.find("}", caret);
		std::string dt = str.substr(caret + 1, ccaret - caret - 1);

		unsigned int n1, n2, n3, n4, n5;
		unsigned int c = 0, cc = 0;
		cc = dt.find(",");
		n1 = StrToInt(dt.substr(c, cc - c).c_str()); c = cc + 1;
		cc = dt.find(",", c);
		n2 = StrToInt(dt.substr(c, cc - c).c_str()); c = cc + 1;
		cc = dt.find(",", c);
		n3 = StrToInt(dt.substr(c, cc - c).c_str()); c = cc + 1;
		cc = dt.find(",", c);
		n4 = StrToInt(dt.substr(c, cc - c).c_str()); c = cc + 1;
		cc = dt.find(",", c);
		n5 = StrToInt(dt.substr(c, cc - c).c_str()); c = cc + 1;

		cDataObject* symbolc = symbolsChild->addChild("symbol");
		symbolc->addChild(n1, "charId");

		fRect rt((float)n2, (float)n3, (float)(n2 + n4), (float)(n3 + n5));
		symbolc->addChild(rt, "texRect");
	}

	getDataObjectsManager().saveDataObject("../data/fonts/test_font.xml", cDataObjectsManager::DOT_XML, dataObj);
}

uiFont& uiFont::setTextAreaSize( const vec2& size )
{
	return setTextArea(fRect(mTextArea.leftTop, mTextArea.leftTop + size));
}

wstring& uiFont::getWText( bool textWillChange /*= true*/ )
{
	mNeedUpdateMesh = textWillChange;
	return mText;
}

uiFont& uiFont::setText( const wstring& text )
{
	mText = text;
	mNeedUpdateMesh = true;
	return *this;
}
