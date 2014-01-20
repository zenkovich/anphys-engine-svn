#ifndef TEXT_H
#define TEXT_H

#include <vector>

#include "public.h"

#include "util/string.h"
#include "util/objects.h"
#include "util/property.h"
#include "util/math/basis.h"
#include "util/math/rect.h"
#include "util/math/color.h"

OPEN_O2_NAMESPACE

class grMesh;
class grFont;
class grRenderSystem;

class grText: public IDrawable
{
public: 
	enum TextStyle { TS_NORMAL   = 1 << 0,
	                 TS_BOLD     = 1 << 1,
					 TS_CURSIVE  = 1 << 2,
					 TS_BORDER   = 1 << 3,
					 TS_SHADOW   = 1 << 4,
					 TS_GRADIENT = 1 << 5  };

	enum VerAlign { VA_TOP, VA_CENTER, VA_BOTTOM, VA_BOTH };
	enum HorAlign { HA_LEFT, HA_CENTER, HA_RIGHT, HA_BOTH };

protected:
	typedef std::vector<grMesh*> MeshVec;

	grRenderSystem* mRenderSystem;

	wstring         mText;
	grFont*         mFont;
	basis           mTransform;
	basisDef        mTransformDef;
	vec2f           mAreaSize;
	float           mCharactersDistCoef;
	float           mLinesDistCoef;
	TextStyle       mStyle;
	VerAlign        mVerAlign;
	HorAlign        mHorAlign;
	bool            mWordWrap;
	color4          mColor;
	color4          mGradientTopColor;
	color4          mGradientBottomColor;
	color4          mBorderColor;
	color4          mShadowColor;
	vec2f           mEffectOffset;
			        
	MeshVec         mMeshes;
	basis           mLastTransform;
	bool            mNeedUpdateMesh;
	bool            mNeedTransformMesh;

public:
	//properties
	PROPERTY(grText, grFont*)     font;
	PROPERTY(grText, wstring)     text;
	PROPERTY(grText, std::string) ctext;
	PROPERTY(grText, TextStyle)   style;
	PROPERTY(grText, VerAlign)    verAlign;
	PROPERTY(grText, HorAlign)    horAlign;
	PROPERTY(grText, bool)        wordWrap;
	PROPERTY(grText, bool)        cursive;
	PROPERTY(grText, bool)        bold;
	PROPERTY(grText, bool)        shadow;
	PROPERTY(grText, bool)        gradient;
	PROPERTY(grText, vec2f)       effectOffset;
	PROPERTY(grText, color4)      color;
	PROPERTY(grText, color4)      borderColor;
	PROPERTY(grText, color4)      shadowColor;
	PROPERTY(grText, color4)      gradientColorTop;
	PROPERTY(grText, color4)      gradientColorBottom;
	PROPERTY(grText, vec2f)       position;
	PROPERTY(grText, float)       angle;
	PROPERTY(grText, vec2f)       scale;
	PROPERTY(grText, float)       charactersHeight;
	PROPERTY(grText, basis)       transform;
	PROPERTY(grText, basisDef)    transformDef;
	PROPERTY(grText, vec2f)       areaSize;
	PROPERTY(grText, float)       charactersDistCoef;
	PROPERTY(grText, float)       linesDistCoef;


	grText(grRenderSystem* renerSystem, grFont* font);
	~grText();

	void draw();	

	void setFont(grFont* const& font);
	grFont* getFont();

	void setText(const wstring& text);
	wstring getText();

	void setText(const std::string& text);
	std::string getCText();

	void setTextStyle(const TextStyle& style);
	TextStyle getTextStyle();

	void setCursive(const bool& flag);
	bool isCursive();

	void setBold(const bool& flag);
	bool isBold();

	void setBorder(const bool& border);
	bool isWithBorder();

	void setShadow(const bool& shadow);
	bool isWithShadow();

	void setGradient(const bool& gradient);
	bool isWithGradient();

	void setEffectOffset(const vec2f& offset);
	vec2f getEffectOffset();

	void setColor(const color4& color);
	color4 getColor();

	void setGradientColors(const color4& topColor, const color4& bottomColor);
	void setGradientTopColor(const color4& color);
	void setGradientBottomColor(const color4& color);

	color4 getGradientTopColor();
	color4 getGradientBottomColor();

	void setShadowColor(const color4& color);
	color4 getShadowColor();

	void setBorderColor(const color4& color);
	color4 getBorderColor();

	void setPosition(const vec2f& position);
	vec2f getPosition();

	void setAngle(const float& angle);
	float getAngle();

	void setScale(const vec2f& scale);
	vec2f getScale();

	void setCharactersHeight(const float& height);
	float getCharactersHeight();

	void setTransform(const basis& bas);
	basis getTransform();

	void setTransformDef(const basisDef& def);
	basisDef getTransformDef();

	void setAreaSize(const vec2f& size);
	vec2f getAreaSize();

	void setHorAlign(const HorAlign& align);
	HorAlign getHorAlign();

	void setVerAlign(const VerAlign& align);
	VerAlign getVerAlign();

	void setWordWrap(const bool& flag);
	bool getWordWrap();

	void setCharactersDistCoef(const float& coef);
	float getCharactersDistCoef();

	void setLinesDistCoef(const float& coef);
	float getLinesDistCoef();

protected:
	void initializeProperties();
	void updateMesh();
	void transformMesh(const basis& bas);
	void prepareMesh(int charactersCount);

	struct symbolDef
	{
		fRect  mPosition;
		fRect  mTexSrc;
		uint16 mCharId;

		symbolDef() {}
		symbolDef(const vec2f& position, const vec2f& size, const fRect& texSrc, uint16 charId):
			mPosition(position, position + size), mTexSrc(texSrc), mCharId(charId) {}
	};
	typedef std::vector<symbolDef> SymbolDefVec;

	struct lineDef
	{
		SymbolDefVec mSymbols;
		wstring      mString;
		float        mSize;
		int          mLineBegSymbol;
		int          mSpacesCount;

		lineDef():mSize(0), mLineBegSymbol(0), mSpacesCount(0) {}
	};
	typedef std::vector<lineDef> LineDefVec;
	
	void pushSymbol(grMesh*& mesh, int& meshIdx, const symbolDef& symb, const vec2f& locOrigin);
	void checkMeshEndless(grMesh*& mesh, int& meshIdx, int size = 2);

	LineDefVec mLineDefs;
};

CLOSE_O2_NAMESPACE

#endif // TEXT_H
