#include "text.h"

#include "render_system.h"
#include "mesh.h"
#include "font.h"

OPEN_O2_NAMESPACE

grText::grText( shared<grFont> font ):
	mFont(font), mCharactersDistCoef(1), mLinesDistCoef(1.5f), mStyle(TS_NORMAL), 
	mVerAlign(VA_TOP), mHorAlign(HA_LEFT), mNeedUpdateMesh(true), mWordWrap(false),
	IRectDrawable()
{
	initializeProperties();
}

grText::~grText()
{
	for (MeshVec::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
		safe_release(*it);
}

void grText::draw()
{
	if (mNeedUpdateMesh)
		updateMesh();

	if (mNeedTransformMesh)
	{
		mLastTransform = mTransform;
		mTransform = mTransformDef.build();
		transformMesh(mLastTransform.inverted()*mTransform);
	}
	
	for (MeshVec::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
		(*it)->draw();
}

void grText::setFont( const shared<grFont>& font )
{
	if (font == mFont)
		return;

	mFont = font;
	mNeedUpdateMesh = true;
}

shared<grFont> grText::getFont() const
{
	return mFont;
}

void grText::setText( const wstring& text )
{
	mText = text;
	mNeedUpdateMesh = true;
}

wstring grText::getText() const
{
	return mText;
}

void grText::setCText( const string& text )
{
	mText = convString2Wide(text);
	mNeedUpdateMesh = true;
}

string grText::getCText() const
{
	return convWide2String(mText);
}

void grText::setTextStyle( const TextStyle& style )
{
	if (mStyle == style)
		return;

	mStyle = style;
	mNeedUpdateMesh = true;
}

grText::TextStyle grText::getTextStyle() const
{
	return mStyle;
}

void grText::setCursive( const bool& flag )
{
	if (flag == isCursive())
		return;

	if (flag)
		mStyle = (TextStyle)(mStyle | TS_CURSIVE);
	else
		mStyle = (TextStyle)(mStyle & ~TS_CURSIVE);

	mNeedUpdateMesh = true;
}

bool grText::isCursive() const
{
	return (TextStyle)(mStyle & TS_CURSIVE) == TS_CURSIVE;
}

void grText::setBold( const bool& flag )
{
	if (flag == isBold())
		return;

	if (flag)
		mStyle = (TextStyle)(mStyle | TS_BOLD);
	else
		mStyle = (TextStyle)(mStyle & ~TS_BOLD);

	mNeedUpdateMesh = true;
}

bool grText::isBold() const
{
	return (TextStyle)(mStyle & TS_BOLD) == TS_BOLD;
}

void grText::setBorder( const bool& border )
{
	if (border == isWithBorder())
		return;

	if (border)
	{
		mStyle = (TextStyle)(mStyle | TS_BORDER);
		mStyle = (TextStyle)(mStyle & ~TS_SHADOW);
	}
	else
		mStyle = (TextStyle)(mStyle & ~TS_BORDER);	

	mNeedUpdateMesh = true;
}

bool grText::isWithBorder() const
{
	return (TextStyle)(mStyle & TS_BORDER) == TS_BORDER;
}

void grText::setShadow( const bool& shadow )
{
	if (shadow == isWithShadow())
		return;

	if (shadow)
	{
		mStyle = (TextStyle)(mStyle | TS_SHADOW);
		mStyle = (TextStyle)(mStyle & ~TS_BORDER);	
	}
	else
		mStyle = (TextStyle)(mStyle & ~TS_SHADOW);

	mNeedUpdateMesh = true;
}

bool grText::isWithShadow() const
{
	return (TextStyle)(mStyle & TS_SHADOW) == TS_SHADOW;
}

void grText::setGradient( const bool& gradient )
{
	if (gradient == isWithGradient())
		return;

	if (gradient)
		mStyle = (TextStyle)(mStyle | TS_GRADIENT);
	else
		mStyle = (TextStyle)(mStyle & ~TS_GRADIENT);

	mNeedUpdateMesh = true;
}

bool grText::isWithGradient() const
{
	return (TextStyle)(mStyle & TS_GRADIENT) == TS_GRADIENT;
}

void grText::setEffectOffset( const vec2f& offset )
{
	if (offset == mEffectOffset)
		return;

	mEffectOffset = offset;
	mNeedUpdateMesh = true;
}

vec2f grText::getEffectOffset() const
{
	return mEffectOffset;
}

void grText::setGradientColors( const color4& topColor, const color4& bottomColor )
{
	if (mGradientTopColor == topColor && mGradientBottomColor == bottomColor)
		return;

	mGradientTopColor = topColor;
	mGradientBottomColor = bottomColor;
	mNeedUpdateMesh = true;
}

void grText::setGradientTopColor( const color4& color )
{
	if (mGradientTopColor == color)
		return;

	mGradientTopColor = color;
	mNeedUpdateMesh = true;
}

void grText::setGradientBottomColor( const color4& color )
{
	if (mGradientBottomColor == color)
		return;

	mGradientBottomColor = color;
	mNeedUpdateMesh = true;
}

color4 grText::getGradientTopColor() const
{
	return mGradientTopColor;
}

color4 grText::getGradientBottomColor() const
{
	return mGradientBottomColor;
}

void grText::setShadowColor( const color4& color )
{
	if (mShadowColor == color)
		return;

	mShadowColor = color;
	mNeedUpdateMesh = true;
}

color4 grText::getShadowColor() const
{
	return mShadowColor;
}

void grText::setBorderColor( const color4& color )
{
	if (mBorderColor == color)
		return;

	mBorderColor = color;
	mNeedUpdateMesh = true;
}

color4 grText::getBorderColor() const
{
	return mBorderColor;
}

vec2f grText::getPosition() const
{
	return mTransformDef.mPosition + mPivot;
}

void grText::setAngle( const float& angle )
{
	if (equals(angle, mTransformDef.mAngle))
		return;

	mTransformDef.mAngle = angle;
	mNeedTransformMesh = true;
}

float grText::getAngle() const
{
	return mTransformDef.mAngle;
}

void grText::setScale( const vec2f& scale )
{
	if (equals(scale, mTransformDef.mScale))
		return;

	mTransformDef.mScale = scale;
	mNeedTransformMesh = true;
}

vec2f grText::getScale() const
{
	return mTransformDef.mScale;
}

void grText::setCharactersHeight( const float& height )
{
	setScale(vec2f(mTransformDef.mScale.x, height/mFont->getLineHeight()));
}

float grText::getCharactersHeight() const
{
	return mTransformDef.mScale.y*mFont->getLineHeight();
}

void grText::setTransform( const basis& bas )
{
	mLastTransform = mTransform;
	mTransform = bas;
	transformMesh(mTransform*mLastTransform.inverted());
}

basis grText::getTransform() const
{
	return mTransform;
}

void grText::setTransformDef( const basisDef& def )
{
	mTransformDef = def;
	mNeedTransformMesh = true;
}

basisDef grText::getTransformDef() const
{
	return mTransformDef;
}

void grText::setHorAlign( const HorAlign& align )
{
	if (align == mHorAlign)
		return;

	mHorAlign = align;
	mNeedUpdateMesh = true;
}

grText::HorAlign grText::getHorAlign() const
{
	return mHorAlign;
}

void grText::setVerAlign( const VerAlign& align )
{
	if (align == mVerAlign)
		return;

	mVerAlign = align;
	mNeedUpdateMesh = true;
}

grText::VerAlign grText::getVerAlign() const
{
	return mVerAlign;
}

void grText::setWordWrap( const bool& flag )
{
	if (flag == mWordWrap)
		return;

	mWordWrap = flag;
	mNeedUpdateMesh = true;
}

bool grText::getWordWrap() const
{
	return mWordWrap;
}

void grText::setCharactersDistCoef( const float& coef )
{
	if (coef == mCharactersDistCoef)
		return;

	mCharactersDistCoef = coef;
	mNeedUpdateMesh = true;
}

float grText::getCharactersDistCoef() const
{
	return mCharactersDistCoef;
}

void grText::setLinesDistCoef( const float& coef )
{
	if (coef == mLinesDistCoef)
		return;

	mLinesDistCoef = coef;
	mNeedUpdateMesh = true;
}

float grText::getLinesDistCoef() const
{
	return mLinesDistCoef;
}

void grText::initializeProperties()
{
	REG_PROPERTY(grText, font, setFont, getFont);
	REG_PROPERTY(grText, text, setText, getText);
	REG_PROPERTY(grText, ctext, setCText, getCText);
	REG_PROPERTY(grText, style, setTextStyle, getTextStyle);
	REG_PROPERTY(grText, verAlign, setVerAlign, getVerAlign);
	REG_PROPERTY(grText, horAlign, setHorAlign, getHorAlign);
	REG_PROPERTY(grText, wordWrap, setWordWrap, getWordWrap);
	REG_PROPERTY(grText, cursive, setCursive, isCursive);
	REG_PROPERTY(grText, bold, setBold, isBold);
	REG_PROPERTY(grText, shadow, setShadow, isWithShadow);
	REG_PROPERTY(grText, border, setBorder, isWithBorder);
	REG_PROPERTY(grText, gradient, setGradient, isWithGradient);
	REG_PROPERTY(grText, effectOffset, setEffectOffset, getEffectOffset);
	REG_PROPERTY(grText, borderColor, setBorderColor, getBorderColor);
	REG_PROPERTY(grText, shadowColor, setShadowColor, getShadowColor);
	REG_PROPERTY(grText, gradientColorTop, setGradientTopColor, getGradientTopColor);
	REG_PROPERTY(grText, gradientColorBottom, setGradientBottomColor, getGradientBottomColor);
	REG_PROPERTY(grText, angle, setAngle, getAngle);
	REG_PROPERTY(grText, scale, setScale, getScale);
	REG_PROPERTY(grText, charactersHeight, setCharactersHeight, getCharactersHeight);
	REG_PROPERTY(grText, transform, setTransform, getTransform);
	REG_PROPERTY(grText, transformDef, setTransformDef, getTransformDef);
	REG_PROPERTY(grText, charactersDistCoef, setCharactersDistCoef, getCharactersDistCoef);
	REG_PROPERTY(grText, linesDistCoef, setLinesDistCoef, getLinesDistCoef);
}

void grText::updateMesh()
{
	mNeedUpdateMesh = false;
	mNeedTransformMesh = false;
	mTransform = mTransformDef.build();

	mLineDefs.clear();

	mLineDefs.push_back(lineDef());
	lineDef* curLine = &mLineDefs.back();
	curLine->mLineBegSymbol = 0;

	int textLen = mText.length();

	prepareMesh(textLen);

	vec2f fullSize;
	bool checkAreaBounds = mWordWrap && mSize.x > FLT_EPSILON;
	int wrapCharIdx = -1;
	for (int i = 0; i < textLen; i++)
	{
		grFont::character* ch = &mFont->mCharacters[mFont->mCharacterIds[mText[i]]];
		vec2f chSize = ch->mSize;
		vec2f chPos = vec2f(curLine->mSize, 0) + ch->mOffset;

		curLine->mSymbols.push_back(symbolDef(chPos, chSize, ch->mTexSrc, ch->mCharId));
		curLine->mSize += ch->mAdvance*mCharactersDistCoef;
		curLine->mString += mText[i];
		
		bool outOfBounds = checkAreaBounds ? curLine->mSize > mSize.x:false;

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
					curLine->mSize = curLine->mSymbols.back().mPosition.right;
				else
					curLine->mSize = 0;

				i = wrapCharIdx;
				wrapCharIdx = -1;
			}

			mLineDefs.push_back(lineDef());
			curLine = &mLineDefs.back();
			curLine->mLineBegSymbol = i + 1;
			fullSize.x = max(fullSize.x, curLine->mSize);
			fullSize.y += mFont->mLineHeight*mLinesDistCoef;
		}
		else if (mText[i] == ' ' || mFont->mAllSymbolReturn)
		{
			curLine->mSpacesCount++;
			wrapCharIdx = i;
		}
	}

	fullSize.x = max(fullSize.x, curLine->mSize);

	for (MeshVec::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
	{
		(*it)->mVertexCount = 0;
		(*it)->mPolyCount = 0;
	}

	int currentMeshIdx = 0;
	grMesh* currentMesh = mMeshes[0];

	float yOffset = 0;
	float lineHeight = mFont->mLineHeight*mLinesDistCoef;

	if (mVerAlign == VA_CENTER)
		yOffset = mSize.y*0.5f - (float)mLineDefs.size()*lineHeight*0.5f;
	else if (mVerAlign == VA_BOTH)
		lineHeight = (mSize.y - lineHeight)/(float)(mLineDefs.size() - 1);
	else if (mVerAlign == VA_BOTTOM)
		yOffset = mSize.y - (float)mLineDefs.size()*lineHeight;

	for (LineDefVec::iterator it = mLineDefs.begin(); it != mLineDefs.end(); ++it)
	{
		lineDef* line = &(*it);
		float xOffset = 0;
		float additiveSpaceOffs = 0;

		if (mHorAlign == HA_CENTER)
			xOffset = (mSize.x - line->mSize)*0.5f;
		else if (mHorAlign == HA_RIGHT)
			xOffset = mSize.x - line->mSize;
		else if (mHorAlign == HA_BOTH)
			additiveSpaceOffs = (mSize.x - line->mSize)/(float)line->mSpacesCount;
			
		vec2f locOrigin( xOffset, yOffset ); 
		yOffset += lineHeight;
		for (SymbolDefVec::iterator jt = line->mSymbols.begin(); jt != line->mSymbols.end(); ++jt)
		{
			if (jt->mCharId == ' ')
				locOrigin.x += additiveSpaceOffs;

			pushSymbol(currentMesh, currentMeshIdx, *jt, locOrigin);
		}
	}
}

void grText::pushSymbol( grMesh*& mesh, int& meshIdx, const symbolDef& symb, 
	                     const vec2f& locOrigin )
{	
	vec2f points[4] = { mTransform.transform(locOrigin + symb.mPosition.getltCorner()),
	                    mTransform.transform(locOrigin + symb.mPosition.getrtCorner()),
	                    mTransform.transform(locOrigin + symb.mPosition.getrdCorner()),
	                    mTransform.transform(locOrigin + symb.mPosition.getldCorner()) };

	if (isBold())
	{
		float offs = symb.mPosition.getSize().x*0.2f;
		points[0].x -= offs; points[3].x -= offs;
		points[1].x += offs; points[2].x += offs;
	}

	if (isCursive())
	{
		float offs = symb.mPosition.getSize().x*0.2f;
		points[0].x += offs; points[1].x += offs;
	}

	if (isWithShadow())
	{
		checkMeshEndless(mesh, meshIdx);

		vec2f transfOffs = mTransform.transform(mEffectOffset) - mTransform.offs;
		vec2f cpoints[4];
		for (int i = 0; i < 4; i++)
			cpoints[i] = points[i] + transfOffs;

		unsigned long shadowColor = mShadowColor.dword();
	
		mesh->mVerticies[mesh->mVertexCount++] = vertex2(cpoints[0], shadowColor, symb.mTexSrc.left, symb.mTexSrc.top);
		mesh->mVerticies[mesh->mVertexCount++] = vertex2(cpoints[1], shadowColor, symb.mTexSrc.right, symb.mTexSrc.top);
		mesh->mVerticies[mesh->mVertexCount++] = vertex2(cpoints[2], shadowColor, symb.mTexSrc.right, symb.mTexSrc.down);
		mesh->mVerticies[mesh->mVertexCount++] = vertex2(cpoints[3], shadowColor, symb.mTexSrc.left, symb.mTexSrc.down);
	
		mesh->mIndexes[mesh->mPolyCount*3    ] = mesh->mVertexCount - 4;
		mesh->mIndexes[mesh->mPolyCount*3 + 1] = mesh->mVertexCount - 3;
		mesh->mIndexes[mesh->mPolyCount*3 + 2] = mesh->mVertexCount - 2;
		mesh->mPolyCount++;
	
		mesh->mIndexes[mesh->mPolyCount*3    ] = mesh->mVertexCount - 4;
		mesh->mIndexes[mesh->mPolyCount*3 + 1] = mesh->mVertexCount - 2;
		mesh->mIndexes[mesh->mPolyCount*3 + 2] = mesh->mVertexCount - 1;
		mesh->mPolyCount++;
	}
	else if (isWithBorder())
	{
		checkMeshEndless(mesh, meshIdx, 8);

		vec2f offsets[4] = { vec2f(-mEffectOffset.x, -mEffectOffset.y), vec2f(mEffectOffset.x, -mEffectOffset.y),
		                     vec2f(mEffectOffset.x, mEffectOffset.y), vec2f(-mEffectOffset.x, mEffectOffset.y) };

		vec2f cpoints[4];

		for (int i = 0; i < 4; i++)
		{
			vec2f transfOffs = mTransform.transform(offsets[i]) - mTransform.offs;
			for (int j = 0; j < 4; j++)
				cpoints[j] = points[j] + transfOffs;

			unsigned long borderColor = mBorderColor.dword();
	
			mesh->mVerticies[mesh->mVertexCount++] = vertex2(cpoints[0], borderColor, symb.mTexSrc.left, symb.mTexSrc.top);
			mesh->mVerticies[mesh->mVertexCount++] = vertex2(cpoints[1], borderColor, symb.mTexSrc.right, symb.mTexSrc.top);
			mesh->mVerticies[mesh->mVertexCount++] = vertex2(cpoints[2], borderColor, symb.mTexSrc.right, symb.mTexSrc.down);
			mesh->mVerticies[mesh->mVertexCount++] = vertex2(cpoints[3], borderColor, symb.mTexSrc.left, symb.mTexSrc.down);
	
			mesh->mIndexes[mesh->mPolyCount*3    ] = mesh->mVertexCount - 4;
			mesh->mIndexes[mesh->mPolyCount*3 + 1] = mesh->mVertexCount - 3;
			mesh->mIndexes[mesh->mPolyCount*3 + 2] = mesh->mVertexCount - 2;
			mesh->mPolyCount++;
	
			mesh->mIndexes[mesh->mPolyCount*3    ] = mesh->mVertexCount - 4;
			mesh->mIndexes[mesh->mPolyCount*3 + 1] = mesh->mVertexCount - 2;
			mesh->mIndexes[mesh->mPolyCount*3 + 2] = mesh->mVertexCount - 1;
			mesh->mPolyCount++;
		}
	}

	checkMeshEndless(mesh, meshIdx);

	unsigned long upColor, bottomColor;
	if (isWithGradient())
	{
		upColor = mGradientTopColor.dword();
		bottomColor = mGradientBottomColor.dword();
	}
	else
		upColor = bottomColor = mColor.dword();
	
	mesh->mVerticies[mesh->mVertexCount++] = vertex2(points[0], upColor,     symb.mTexSrc.left, symb.mTexSrc.top);
	mesh->mVerticies[mesh->mVertexCount++] = vertex2(points[1], upColor,     symb.mTexSrc.right, symb.mTexSrc.top);
	mesh->mVerticies[mesh->mVertexCount++] = vertex2(points[2], bottomColor, symb.mTexSrc.right, symb.mTexSrc.down);
	mesh->mVerticies[mesh->mVertexCount++] = vertex2(points[3], bottomColor, symb.mTexSrc.left, symb.mTexSrc.down);
	
	mesh->mIndexes[mesh->mPolyCount*3    ] = mesh->mVertexCount - 4;
	mesh->mIndexes[mesh->mPolyCount*3 + 1] = mesh->mVertexCount - 3;
	mesh->mIndexes[mesh->mPolyCount*3 + 2] = mesh->mVertexCount - 2;
	mesh->mPolyCount++;
	
	mesh->mIndexes[mesh->mPolyCount*3    ] = mesh->mVertexCount - 4;
	mesh->mIndexes[mesh->mPolyCount*3 + 1] = mesh->mVertexCount - 2;
	mesh->mIndexes[mesh->mPolyCount*3 + 2] = mesh->mVertexCount - 1;
	mesh->mPolyCount++;
}

void grText::checkMeshEndless( grMesh*& mesh, int& meshIdx, int size )
{
	if (mesh->mPolyCount + size >= mesh->getMaxPolyCount())
		mesh = mMeshes[meshIdx++];
}

void grText::prepareMesh( int charactersCount )
{
	int effectsCoef = 1;
	if (isWithShadow())
		effectsCoef += 1;
	if (isWithBorder())
		effectsCoef += 4;

	int needPolygons = charactersCount*2*effectsCoef;
	for (MeshVec::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
		needPolygons -= (*it)->maxPolyCount;

	if (needPolygons <= 0)
		return;

	if (needPolygons < 100 && mMeshes.size() > 0 && 
		needPolygons + mMeshes.back()->maxPolyCount < nMeshMaxPolyCount)
	{		
		mMeshes.back()->resize(mMeshes.back()->maxVertexCount + (uint32)needPolygons*2, 
			                   mMeshes.back()->maxPolyCount + (uint32)needPolygons);
		return;
	}
	
	for (int i = 0; i < charactersCount/needPolygons + 1; i++)
		mMeshes.push_back(mnew grMesh(mFont->mTexture, nMeshMaxPolyCount*2, nMeshMaxPolyCount));
}

void grText::transformMesh( const basis& bas )
{
	mNeedTransformMesh = false;

	FOREACH(MeshVec, mMeshes, it)
	{
		grMesh* mesh = *it;
		for (unsigned int i = 0; i < mesh->mVertexCount; i++)
		{
			vertex2* vx = &mesh->mVerticies[i];
			bas.transform(vx->x, vx->y);
		}
	}
}

void grText::positionChanged()
{
	mTransformDef.mPosition = mPosition - mPivot;
	mNeedTransformMesh = true;
}

void grText::sizeChanged()
{
	mNeedUpdateMesh = true;
}

void grText::pivotChanged()
{
	mTransformDef.mPosition = mPosition - mPivot;
	mNeedTransformMesh = true;
}

void grText::colorChanged()
{
	mNeedUpdateMesh = true;
}

CLOSE_O2_NAMESPACE