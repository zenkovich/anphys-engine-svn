#include "text.h"

#include "render_system.h"
#include "mesh.h"
#include "font.h"

OPEN_O2_NAMESPACE

grText::grText( grFont* font ):
	mFont(font), mCharactersDistCoef(1), mLinesDistCoef(1.5f), mVerAlign(VA_TOP), mHorAlign(HA_LEFT), 
	mNeedUpdateMesh(true), mWordWrap(false), IRectDrawable()
{
	initializeProperties();
}

grText::grText( const grText& text ):
	IRectDrawable(text)
{
	mText = text.mText;               
	mFont = text.mFont;               
	mTransform = text.mTransform;          
	mTransformDef = text.mTransformDef;       
	mCharactersDistCoef = text.mCharactersDistCoef; 
	mLinesDistCoef = text.mLinesDistCoef;  
	mVerAlign = text.mVerAlign;           
	mHorAlign = text.mHorAlign;           
	mWordWrap = text.mWordWrap;        
	
	mNeedUpdateMesh = true;     
	mNeedTransformMesh = false;  

	initializeProperties();
}

grText::~grText()
{
	for (MeshVec::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
		safe_release(*it);
}

IRectDrawable* grText::clone() const
{
	return mnew grText(*this);
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

void grText::setFont( grFont* font )
{
	if (font == mFont)
		return;

	mFont = font;
	mNeedUpdateMesh = true;
}

grFont* grText::getFont() const
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
	setScale(vec2f(mTransformDef.mScale.x, height/mFont->getBase()));
}

float grText::getCharactersHeight() const
{
	return mTransformDef.mScale.y*mFont->getBase();
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
	REG_PROPERTY_SETTER_NONCONST(grText, font, setFont, getFont);
	REG_PROPERTY(grText, text, setText, getText);
	REG_PROPERTY(grText, ctext, setCText, getCText);
	REG_PROPERTY(grText, verAlign, setVerAlign, getVerAlign);
	REG_PROPERTY(grText, horAlign, setHorAlign, getHorAlign);
	REG_PROPERTY(grText, wordWrap, setWordWrap, getWordWrap);
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
	int textLen = mText.length();

	if (mMeshes.size() == 0 && textLen == 0)
		return;

	mLineDefs.push_back(lineDef());
	lineDef* curLine = &mLineDefs.back();

	prepareMesh(textLen);

	vec2f fullSize(0, mFont->getBase());
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
					curLine->mSize = curLine->mSymbols.back().mFrame.right;
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
		yOffset = mSize.y*0.5f - fullSize.y*0.5f + mFont->getBase() - mFont->getLineHeight();
	else if (mVerAlign == VA_BOTH)
		lineHeight = (mSize.y - lineHeight)/(float)(mLineDefs.size() - 1);
	else if (mVerAlign == VA_BOTTOM)
		yOffset = mSize.y - fullSize.y;

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
			
		vec2f locOrigin( (float)(int)xOffset, (float)(int)yOffset ); 
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
	checkMeshEndless(mesh, meshIdx);

	vec2f points[4] = { mTransform.transform(locOrigin + symb.mFrame.getltCorner()),
	                    mTransform.transform(locOrigin + symb.mFrame.getrtCorner()),
	                    mTransform.transform(locOrigin + symb.mFrame.getrdCorner()),
	                    mTransform.transform(locOrigin + symb.mFrame.getldCorner()) };	

	unsigned long color = mColor.dword();
	
	mesh->mVerticies[mesh->mVertexCount++] = vertex2(points[0], color, symb.mTexSrc.left,  symb.mTexSrc.top);
	mesh->mVerticies[mesh->mVertexCount++] = vertex2(points[1], color, symb.mTexSrc.right, symb.mTexSrc.top);
	mesh->mVerticies[mesh->mVertexCount++] = vertex2(points[2], color, symb.mTexSrc.right, symb.mTexSrc.down);
	mesh->mVerticies[mesh->mVertexCount++] = vertex2(points[3], color, symb.mTexSrc.left,  symb.mTexSrc.down);
	
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
	int needPolygons = charactersCount*2;
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