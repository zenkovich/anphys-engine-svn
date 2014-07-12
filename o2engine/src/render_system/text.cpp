#include "text.h"

#include "render_system.h"
#include "mesh.h"
#include "font.h"

OPEN_O2_NAMESPACE

grText::grText( grFont* font ):
	mFont(font), mCharactersDistCoef(1), mLinesDistCoef(1.5f), mVerAlign(grFont::VA_TOP), mHorAlign(grFont::HA_LEFT), 
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
	if (!mEnabled)
		return;

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

void grText::setHorAlign( const grFont::HorAlign& align )
{
	if (align == mHorAlign)
		return;

	mHorAlign = align;
	mNeedUpdateMesh = true;
}

grFont::HorAlign grText::getHorAlign() const
{
	return mHorAlign;
}

void grText::setVerAlign( const grFont::VerAlign& align )
{
	if (align == mVerAlign)
		return;

	mVerAlign = align;
	mNeedUpdateMesh = true;
}

grFont::VerAlign grText::getVerAlign() const
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

grFont::TextSymbolsSet* grText::getSymbolsSet()
{
	return &mSymbolsSet;
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

	int textLen = mText.length();
	if (mMeshes.size() == 0 && textLen == 0)
		return;
	
	prepareMesh(textLen);
	
	FOREACH(MeshVec, mMeshes, it)
	{
		(*it)->mVertexCount = 0; (*it)->mPolyCount = 0;
	}

	int currentMeshIdx = 0;
	grMesh* currentMesh = mMeshes[0];

	mSymbolsSet.initialize(font, mText, mPosition, mSize, mHorAlign, mVerAlign, mWordWrap, mCharactersDistCoef, mLinesDistCoef);
	basis zeroPositionTransform = mTransform;
	zeroPositionTransform.offs = vec2f();

	FOREACH(grFont::TextSymbolsSet::LineDefVec, mSymbolsSet.mLineDefs, it)
	{
		FOREACH(grFont::TextSymbolsSet::SymbolDefVec, it->mSymbols, jt)
		{			
			if (currentMesh->mPolyCount + 2 >= currentMesh->getMaxPolyCount())
				currentMesh = mMeshes[currentMeshIdx++];

			unsigned long color = mColor.dword();
			vec2f points[4] = { zeroPositionTransform.transform(jt->mFrame.getltCorner()),
								zeroPositionTransform.transform(jt->mFrame.getrtCorner()),
								zeroPositionTransform.transform(jt->mFrame.getrdCorner()),
								zeroPositionTransform.transform(jt->mFrame.getldCorner()) };	
	
			currentMesh->mVerticies[currentMesh->mVertexCount++] = vertex2(points[0], color, jt->mTexSrc.left,  jt->mTexSrc.top);
			currentMesh->mVerticies[currentMesh->mVertexCount++] = vertex2(points[1], color, jt->mTexSrc.right, jt->mTexSrc.top);
			currentMesh->mVerticies[currentMesh->mVertexCount++] = vertex2(points[2], color, jt->mTexSrc.right, jt->mTexSrc.down);
			currentMesh->mVerticies[currentMesh->mVertexCount++] = vertex2(points[3], color, jt->mTexSrc.left,  jt->mTexSrc.down);
			
			currentMesh->mIndexes[currentMesh->mPolyCount*3    ] = currentMesh->mVertexCount - 4;
			currentMesh->mIndexes[currentMesh->mPolyCount*3 + 1] = currentMesh->mVertexCount - 3;
			currentMesh->mIndexes[currentMesh->mPolyCount*3 + 2] = currentMesh->mVertexCount - 2;
			currentMesh->mPolyCount++;
			
			currentMesh->mIndexes[currentMesh->mPolyCount*3    ] = currentMesh->mVertexCount - 4;
			currentMesh->mIndexes[currentMesh->mPolyCount*3 + 1] = currentMesh->mVertexCount - 2;
			currentMesh->mIndexes[currentMesh->mPolyCount*3 + 2] = currentMesh->mVertexCount - 1;
			currentMesh->mPolyCount++;
		}
	}
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