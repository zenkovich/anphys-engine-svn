#include "font.h"

#include "render_system.h"
#include "texture.h"
#include "util/xml_tools.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

grFont::grFont():
	mCharacters(NULL), mCharacterIds(NULL), mCharactersCount(0), mTexture(NULL), mAllSymbolReturn(false)
{
}

grFont::grFont( const grFont& font ):
	mCharacters(NULL), mCharacterIds(NULL), mCharactersCount(0), mTexture(NULL)
{
}

grFont::~grFont()
{
	safe_release_arr(mCharacters);
	safe_release_arr(mCharacterIds);
}

void grFont::create( const string& fontFile )
{
	pugi::xml_document doc;
	XmlTools::loadFromFile(fontFile, doc);
}

void grFont::createFromBMFont( const string& fontFile )
{
	pugi::xml_document doc;
	if (!XmlTools::loadFromFile(fontFile, doc))
	{
		renderSystem()->mLog->error("Failed to load BMFont file: %s", fontFile.c_str());
		return;
	}

	pugi::xml_node root = doc.child("font");

	if (pugi::xml_node commonNode = root.child("common"))
	{
		mName = commonNode.attribute("name").value();

		string textureName = commonNode.attribute("texture").value();	
		mTexture = renderSystem()->getTextureFromFile(textureName);

		mLineHeight = commonNode.attribute("lineHeight").as_float();
		mBase = commonNode.attribute("base").as_float();
	}
	else
	{
		renderSystem()->mLog->error("Failed to get common info in font: %s. Bad file format", fontFile.c_str());
		return;
	}

	if (pugi::xml_node charsNode = root.child("chars"))
	{
		mCharactersCount = charsNode.attribute("count").as_int();

		safe_release_arr(mCharacters);
		safe_release_arr(mCharacterIds);
		mCharacters = mnew character[mCharactersCount];
		mCharacterIds = mnew uint16[nMaxSymbolId];
		memset(mCharacterIds, 0, sizeof(uint16)*nMaxSymbolId);

		int i = 0;
		for (pugi::xml_node charNode = charsNode.child("char"); charNode; charNode = charNode.next_sibling("char"))
		{
			character* charp = &(mCharacters[i++]);

			charp->mTexSrc.left = charNode.attribute("x").as_float();
			charp->mTexSrc.top = charNode.attribute("y").as_float();
			charp->mTexSrc.right = charNode.attribute("width").as_float() + charp->mTexSrc.left;
			charp->mTexSrc.down = charNode.attribute("height").as_float() + charp->mTexSrc.top;
			
			charp->mOffset.x = charNode.attribute("xoffset").as_float();
			charp->mOffset.y = charNode.attribute("yoffset").as_float();
			
			charp->mAdvance = charNode.attribute("xadvance").as_float();

			charp->mCharId = charNode.attribute("id").as_uint();
		}
		
		for (int i = 0; i < mCharactersCount; i++)
			mCharacterIds[mCharacters[i].mCharId] = i;
	}
	else
	{
		renderSystem()->mLog->error("Failed to get characters node in BMFont file: %s. Bad file format", fontFile.c_str());
		return;
	}

	vec2f invTexSize(1.0f/mTexture.getSize().x, 1.0f/mTexture.getSize().y);
	for (int i = 0; i < mCharactersCount; i++)
	{
		mCharacters[i].mSize = mCharacters[i].mTexSrc.getSize();
		mCharacters[i].mTexSrc.left *= invTexSize.x;
		mCharacters[i].mTexSrc.right *= invTexSize.x;
		mCharacters[i].mTexSrc.top *= invTexSize.y;
		mCharacters[i].mTexSrc.down *= invTexSize.y;
	}
}

float grFont::getLineHeight() const
{
	return mLineHeight;
}

float grFont::getBase() const
{
	return mBase;
}

vec2f grFont::getTextSize(const wstring& text, const vec2f& areaSize /*= vec2f()*/, HorAlign horAlign /*= HA_LEFT*/,
	                      VerAlign verAlign /*= VA_TOP*/, bool wordWrap /*= true*/, float charsDistCoef /*= 1.0f*/, 
						  float linesDistCoef /*= 1.0f*/)
{
	TextSymbolsSet textSet;
	textSet.initialize(this, text, vec2f(), areaSize, horAlign, verAlign, wordWrap, charsDistCoef, linesDistCoef);
	return textSet.mRealSize;
}

float grFont::getSymbolAdvance( uint16 id )
{
	return mCharacters[ mCharacterIds[id] ].mAdvance;
}


void grFont::TextSymbolsSet::initialize(grFont* font, const wstring& text, const vec2f& position, const vec2f& areaSize,
	                                    HorAlign horAlign, VerAlign verAlign, bool wordWrap, float charsDistCoef, 
										float linesDistCoef)
{
	mFont = font;
	mText = text;
	mPosition = position;
	mAreaSize = areaSize;
	mRealSize = vec2f();
	mHorAlign = horAlign;
	mVerAlign = verAlign;
	mWordWrap = wordWrap;
	mCharactersDistCoef = charsDistCoef;
	mLinesDistCoef = linesDistCoef;

	mLineDefs.clear();
	int textLen = mText.length();

	if (textLen == 0)
		return;

	mLineDefs.push_back(lineDef());
	lineDef* curLine = &mLineDefs.back();
	curLine->mSize.y = mFont->getLineHeight();
	
	vec2f fullSize(0, mFont->getBase());
	bool checkAreaBounds = mWordWrap && mAreaSize.x > FLT_EPSILON;
	int wrapCharIdx = -1;
	for (int i = 0; i < textLen; i++)
	{
		grFont::character* ch = &mFont->mCharacters[mFont->mCharacterIds[mText[i]]];
		vec2f chSize = ch->mSize;
		vec2f chPos = vec2f(curLine->mSize.x, 0) + ch->mOffset;

		curLine->mSymbols.push_back(symbolDef(chPos, chSize, ch->mTexSrc, ch->mCharId, ch->mOffset, ch->mAdvance));
		curLine->mSize.x += ch->mAdvance*mCharactersDistCoef;
		curLine->mString += mText[i];
		
		bool outOfBounds = checkAreaBounds ? curLine->mSize.x > mAreaSize.x:false;

		if (mText[i] == '\n' || outOfBounds)
		{
			if (outOfBounds)
			{
				if (wrapCharIdx < 0)
					wrapCharIdx = i;
				else
					curLine->mSpacesCount--;

				int cutLen = wrapCharIdx - curLine->mLineBegSymbol;

				curLine->mSymbols.erase(curLine->mSymbols.begin() + cutLen, curLine->mSymbols.end());
				curLine->mString.erase(curLine->mString.begin() + cutLen, curLine->mString.end());


				if (curLine->mSymbols.size() > 0) 
					curLine->mSize.x = curLine->mSymbols.back().mFrame.right;
				else
					curLine->mSize.x = 0;

				i = wrapCharIdx;
				wrapCharIdx = -1;
			}
			else 
			{
				curLine->mSymbols.pop_back();
				curLine->mString.pop_back();
				curLine->mEndedNewLine = true;
			}

			mLineDefs.push_back(lineDef());
			curLine = &mLineDefs.back();
			curLine->mSize.y = mFont->getLineHeight();
			curLine->mLineBegSymbol = i + 1;
			fullSize.x = max(fullSize.x, curLine->mSize.x);
			fullSize.y += mFont->mLineHeight*mLinesDistCoef;
		}
		else if (mText[i] == ' ' || mFont->mAllSymbolReturn)
		{
			curLine->mSpacesCount++;
			wrapCharIdx = i;
		}
	}

	fullSize.x = max(fullSize.x, curLine->mSize.x);

	float yOffset = 0;
	float lineHeight = mFont->mLineHeight*mLinesDistCoef;

	if (mVerAlign == VA_CENTER)
		yOffset = mAreaSize.y*0.5f - fullSize.y*0.5f + mFont->getBase() - mFont->getLineHeight();
	else if (mVerAlign == VA_BOTH)
		lineHeight = (mAreaSize.y - lineHeight + mFont->getBase())/(float)(mLineDefs.size() - 1);
	else if (mVerAlign == VA_BOTTOM)
		yOffset = mAreaSize.y - fullSize.y;

	yOffset += mPosition.y;

	for (LineDefVec::iterator it = mLineDefs.begin(); it != mLineDefs.end(); ++it)
	{
		lineDef* line = &(*it);
		float xOffset = 0;
		float additiveSpaceOffs = 0;

		if (mHorAlign == HA_CENTER)
			xOffset = (mAreaSize.x - line->mSize.x)*0.5f;
		else if (mHorAlign == HA_RIGHT)
			xOffset = mAreaSize.x - line->mSize.x;
		else if (mHorAlign == HA_BOTH)
			additiveSpaceOffs = (mAreaSize.x - line->mSize.x)/(float)line->mSpacesCount;

		xOffset += mPosition.x;
			
		vec2f locOrigin( (float)(int)xOffset, (float)(int)yOffset ); 
		line->mPosition = locOrigin;
		yOffset += lineHeight;
		for (SymbolDefVec::iterator jt = line->mSymbols.begin(); jt != line->mSymbols.end(); ++jt)
		{
			if (jt->mCharId == ' ')
				locOrigin.x += additiveSpaceOffs;

			jt->mFrame = jt->mFrame + locOrigin;
		}
		
		mRealSize.x = max(mRealSize.x, line->mSize.x);
		mRealSize.y = max(mRealSize.y, line->mSize.y + line->mPosition.y - mPosition.y);
	}
}

CLOSE_O2_NAMESPACE