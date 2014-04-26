#ifndef TEXT_H
#define TEXT_H

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

/** Text renderer class. Using font, basis and many style parametres. Caching text into meshes. */
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

	enum { nMeshMaxPolyCount = 2048 };

protected:
	typedef vector<grMesh*> MeshVec;

	wstring         mText;                /** Wide char string, containing rendering text. */
	grFont*         mFont;                /** Using font. */
	basis           mTransform;           /** Transformation. */
	basisDef        mTransformDef;        /** Transform definition. */
	vec2f           mAreaSize;            /** Size of text area. */
	float           mCharactersDistCoef;  /** Characters distance coef, 1 is standart. */
	float           mLinesDistCoef;       /** Lines distance coef, 1 is standart. */
	TextStyle       mStyle;               /** Style bit mask. */
	VerAlign        mVerAlign;            /** Vertical align. */
	HorAlign        mHorAlign;            /** Horisontal align. */
	bool            mWordWrap;            /** True, when words wrapping. */
	color4          mColor;               /** Color of the text. */
	color4          mGradientTopColor;    /** Gradient effect top color. */
	color4          mGradientBottomColor; /** Gradient effect bottom color. */
	color4          mBorderColor;         /** Border effect color. */
	color4          mShadowColor;         /** Shadow effect color. */
	vec2f           mEffectOffset;        /** Effects offset. */
			        
	MeshVec         mMeshes;              /** Meshes vector. */
	basis           mLastTransform;       /** Last mesh update transformation. */
	bool            mNeedUpdateMesh;      /** True, when need rebuild meshes. */
	bool            mNeedTransformMesh;   /** True, whe need transform meshes. */

public:
	//properties
	PROPERTY(grText, grFont*)     font;                /** Font pointer property. Uses set/getFont. */
	PROPERTY(grText, wstring)     text;                /** Text property, wstring. Uses set/getText. */
	PROPERTY(grText, string) ctext;               /** Text property, string. Uses set/getCText. */
	PROPERTY(grText, TextStyle)   style;               /** Style bit mask property. Uses set/gettextStyle. */
	PROPERTY(grText, VerAlign)    verAlign;            /** vertical align property. Uses set/getVerAlign. */
	PROPERTY(grText, HorAlign)    horAlign;            /** Horisontal align property. Uses set/getHorAlign. */
	PROPERTY(grText, bool)        wordWrap;            /** Words wrapping flag property. Uses set/getWordWrap. */
	PROPERTY(grText, bool)        cursive;             /** Cursive flag property. Uses set/isCursive. */
	PROPERTY(grText, bool)        bold;                /** Bold flag property. Uses set/isBold. */
	PROPERTY(grText, bool)        shadow;              /** Shadow flag property. Uses setShadow/isWithShadow. */
	PROPERTY(grText, bool)        border;              /** Border flag property. Uses setBorder/isWithBorder. */
	PROPERTY(grText, bool)        gradient;            /** Gradient flag property. Uses setGradient/isWithGradient. */
	PROPERTY(grText, vec2f)       effectOffset;        /** Effects offset property. Uses set/getEffectsOffset. */
	PROPERTY(grText, color4)      color;               /** Text color property. Uses set/getColor. */
	PROPERTY(grText, color4)      borderColor;         /** Border color property. Uses set/getBorderColor. */
	PROPERTY(grText, color4)      shadowColor;         /** Shadow color property. Uses set/getShadowColor. */
	PROPERTY(grText, color4)      gradientColorTop;    /** Gradient top color property. Uses set/getGradientBottomColor. */
	PROPERTY(grText, color4)      gradientColorBottom; /** Gradient bottom color property. Uses set/getGradientBottomColor. */
	PROPERTY(grText, vec2f)       position;            /** Position property. Uses set/getPosition. */
	PROPERTY(grText, float)       angle;               /** Angle of rotation property. Uses set/getAngle. */
	PROPERTY(grText, vec2f)       scale;               /** Scale property. Uses set/getScale. */
	PROPERTY(grText, float)       charactersHeight;    /** Characters height property, pixels. Uses set/getCharactersHeight. */
	PROPERTY(grText, basis)       transform;           /** Transformation property. Uses set/getTransform. */
	PROPERTY(grText, basisDef)    transformDef;        /** Transform definition property. Uses set/getTransformDef. */
	PROPERTY(grText, vec2f)       areaSize;            /** Area size property. Uses set/getAreaSize. */
	PROPERTY(grText, fRect)       rect;                /** Text rectangle area. Uses set/getRect. */
	PROPERTY(grText, float)       charactersDistCoef;  /** Characters distance coef, 1 is standart. Uses set/getCharactersDistCoef. */
	PROPERTY(grText, float)       linesDistCoef;       /** Lines distance coef, 1 is standart. Uses set/getLinesDistCoef. */

	/** ctor. */
	grText(grFont* font);

	/** dtor. */
	~grText();

	/** Draw text. */
	void draw();	

	/** Sets using font. */
	void setFont(grFont* const& font);

	/** Returns using font. */
	grFont* getFont() const;

	/** Sets text. */
	void setText(const wstring& text);

	/** Returns text. */
	wstring getText() const;

	/** Sets text. */
	void setCText(const string& text);

	/** Returns text as string. */
	string getCText() const;

	/** Sets text style bit mask. */
	void setTextStyle(const TextStyle& style);

	/** Returns text style bit mask. */
	TextStyle getTextStyle() const;

	/** Sets text cursive effect. */
	void setCursive(const bool& flag);

	/** Returns true when cursive enabled. */
	bool isCursive() const;

	/** Sets text bold effect. */
	void setBold(const bool& flag);

	/** Returns true when bold enabled. */
	bool isBold() const;

	/** Sets border. */
	void setBorder(const bool& border);

	/** Returns true when border enabled. */
	bool isWithBorder() const;

	/** Sets gradient. */
	void setShadow(const bool& shadow);

	/** Returns true when shadow enabled. */
	bool isWithShadow() const;

	/** Sets gradient effect. */
	void setGradient(const bool& gradient);

	/** Returns true when gradient enabled. */
	bool isWithGradient() const;

	/** Sets effects offset. */
	void setEffectOffset(const vec2f& offset);

	/** Returns effects offset. */
	vec2f getEffectOffset() const;

	/** Sets text color. */
	void setColor(const color4& color);

	/** Returns text color. */
	color4 getColor() const;

	/** Sets gradient colors. */
	void setGradientColors(const color4& topColor, const color4& bottomColor);

	/** Sets gradient top color. */
	void setGradientTopColor(const color4& color);

	/** Sets gradient bottom color. */
	void setGradientBottomColor(const color4& color);

	/** Returns gradient top color. */
	color4 getGradientTopColor() const;

	/** Retuns gradient bottom color. */
	color4 getGradientBottomColor() const;

	/** Sets shadow color. */
	void setShadowColor(const color4& color);

	/** Returns shadow color. */
	color4 getShadowColor() const;

	/** Sets border color. */
	void setBorderColor(const color4& color);

	/** Returns border color. */
	color4 getBorderColor() const;

	/** Sets position of text. */
	void setPosition(const vec2f& position);

	/** Returns position of text. */
	vec2f getPosition() const;

	/** Sets rotation angle. */
	void setAngle(const float& angle);

	/** Returns rotation angle. */
	float getAngle() const;

	/** Sets text scale. */
	void setScale(const vec2f& scale);

	/** Returns text scale. */
	vec2f getScale() const;

	/** Sets characters height in pixels. */
	void setCharactersHeight(const float& height);

	/** Returns characters height in pixels. */
	float getCharactersHeight() const;

	/** Sets transformation of text. */
	void setTransform(const basis& bas);

	/** Returns text transformation. */
	basis getTransform() const;

	/** Sets text transformation defnition. */
	void setTransformDef(const basisDef& def);

	/** Returns text transformation definition. */
	basisDef getTransformDef() const;

	/** Sets text area size/ */
	void setAreaSize(const vec2f& size);

	/** Returns area size. */
	vec2f getAreaSize() const;

	/** Sets text rectange (position and area size). */
	void setRect(const fRect& rect);

	/** Returns text rectangle. */
	fRect getRect() const;

	/** Sets gorisontal oalign. */
	void setHorAlign(const HorAlign& align);

	/** Returns horisontal align. */
	HorAlign getHorAlign() const;

	/** Sets vertical align. */
	void setVerAlign(const VerAlign& align);

	/** returns vertical align. */
	VerAlign getVerAlign() const;

	/** Sets word wrapping. */
	void setWordWrap(const bool& flag);

	/** Returns word wrapping. */
	bool getWordWrap() const;

	/** Sets characters distance coeficient. */
	void setCharactersDistCoef(const float& coef = 1);

	/** Returns characters distance coef. */
	float getCharactersDistCoef() const;

	/** Sets lines distance coeficient. */
	void setLinesDistCoef(const float& coef = 1);

	/** Returns lines distance coeficient. */
	float getLinesDistCoef() const;

protected:
	/** Initializing properties. */
	void initializeProperties();

	/** Updating meshes. */
	void updateMesh();

	/** Transforming meshes by basis. */
	void transformMesh(const basis& bas);

	/** Preparing meshes for characters count. */
	void prepareMesh(int charactersCount);

	/** Simple symol definition structure. */
	struct symbolDef
	{
		fRect  mPosition; /** Rect positioning. */
		fRect  mTexSrc;   /** Texture src rect. */
		uint16 mCharId;   /** Character id. */

		symbolDef() {}
		symbolDef(const vec2f& position, const vec2f& size, const fRect& texSrc, uint16 charId):
			mPosition(position, position + size), mTexSrc(texSrc), mCharId(charId) {}
	};
	typedef vector<symbolDef> SymbolDefVec;

	/** Line definition structure. */
	struct lineDef
	{
		SymbolDefVec mSymbols;       /** Symbols in line. */
		wstring      mString;        /** Line string. */
		float        mSize;          /** Size of line in pixels. */
		int          mLineBegSymbol; /** Index of line beginning symbol. */
		int          mSpacesCount;   /** Spaces count at line. */

		lineDef():mSize(0), mLineBegSymbol(0), mSpacesCount(0) {}
	};
	typedef vector<lineDef> LineDefVec;
	
	/** Push symbol to meshes. */
	void pushSymbol(grMesh*& mesh, int& meshIdx, const symbolDef& symb, const vec2f& locOrigin);

	/** Check mesh endless. If at end, take new mesh. */
	void checkMeshEndless(grMesh*& mesh, int& meshIdx, int size = 2);

	LineDefVec mLineDefs; /** Lines definitions. */
};

CLOSE_O2_NAMESPACE

#endif // TEXT_H
