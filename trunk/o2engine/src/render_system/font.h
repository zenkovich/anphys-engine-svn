#ifndef FONT_H
#define FONT_H

#include "public.h"

#include "texture.h"
#include "util/math/rect.h"
#include "util/serialization/serialization.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class grMesh;
class grRenderSystem;

/** Font. Containing array of symbol glyphs, symbol index table. */
class grFont: public cSerializableObj
{
	friend class grFontManager;
	friend class grText;
	
public:
	enum VerAlign { VA_TOP, VA_CENTER, VA_BOTTOM, VA_BOTH };
	enum HorAlign { HA_LEFT, HA_CENTER, HA_RIGHT, HA_BOTH };

	/** Characret definition structure. Conatining texture source rect, size, offset, advance and characted id. */
	struct character: public cSerializableObj
	{
		fRect  mTexSrc;  /** texture source rect. */
		vec2f  mSize;    /** Size of source rect. */
		vec2f  mOffset;  /** Symbol offset. */
		float  mAdvance; /** Symbol advance. */
		uint16 mCharId;  /** Character id. */

		/** Serialization method. */
		SERIALIZE_METHOD_DECL();
	};

	/** text symbols layouts set. */
	struct TextSymbolsSet
	{
		/** Simple symbol definition structure. */
		struct symbolDef
		{
			fRect  mFrame;  /** Frame of symbol layout. */
			fRect  mTexSrc; /** Texture source rect. */
			uint16 mCharId; /** Character id. */
			vec2f  mOffset; /** Character offset. */
			float  mAdvance;/** Character advance. */

			symbolDef() {}
			symbolDef(const vec2f& position, const vec2f& size, const fRect& texSrc, uint16 charId, const vec2f& offs, float advance):
				mFrame(position, position + size), mTexSrc(texSrc), mCharId(charId), mOffset(offs), mAdvance(advance) {}
		};
		typedef vector<symbolDef> SymbolDefVec;

		/** Line definition structure. */
		struct lineDef
		{
			SymbolDefVec mSymbols;       /** Symbols in line. */
			wstring      mString;        /** Line string. */
			vec2f        mSize;          /** Size of line in pixels. */
			vec2f        mPosition;      /** Position of line. */
			int          mLineBegSymbol; /** Index of line beginning symbol. */
			int          mSpacesCount;   /** Spaces count at line. */

			lineDef():mLineBegSymbol(0), mSpacesCount(0) {}
		};
		typedef vector<lineDef> LineDefVec;

		grFont*    mFont;
		wstring    mText;               /** Text string. */
		vec2f      mPosition;           /** Position, in pixels. */
		vec2f      mAreaSize;           /** Area size, in pixels. */
		vec2f      mRealSize;           /** Real text size. */
		HorAlign   mHorAlign;           /** Horizontal align. */
		VerAlign   mVerAlign;           /** Vertical align. */
		bool       mWordWrap;           /** True, when words wrapping. */
		float      mCharactersDistCoef; /** Characters distance coef, 1 is standard. */
		float      mLinesDistCoef;      /** Lines distance coef, 1 is standard. */
		LineDefVec mLineDefs;           /** Lines definitions. */

		/** Calculating characters layout by parameters. */
		void initialize(grFont* font, const wstring& text, const vec2f& position, const vec2f& areaSize, HorAlign horAlign,
			            VerAlign verAlign, bool wordWrap, float charsDistCoef, float linesDistCoef);
	};

protected:
	enum { nMaxSymbolId = 0xffff };

	string      mName;            /** Font name. */
	character*  mCharacters;      /** Characters array. */
	int         mCharactersCount; /** Characters count in array. */
	bool        mAllSymbolReturn; /** True, when all symbols is new line symbols. */
	float       mLineHeight;      /** Line height, in pixels. */
	float       mBase;            /** Base, in pixels. */
	uint16*     mCharacterIds;    /** Characters ids array. */
	grTexture   mTexture;         /** Texture. */

	/** ctor. */
	grFont();

	/** deprecate copy-ctor. */
	grFont(const grFont& font);

	/** dtor. */
	~grFont();

	/** Create font from config file. */
	void create(const string& fontFile);

	/** Create font from BMFont config file. */
	void createFromBMFont(const string& fontFile);

	vec2f getTextSize(const wstring& text, const vec2f& areaSize = vec2f(), HorAlign horAlign = HA_LEFT,
			          VerAlign verAlign = VA_TOP, bool wordWrap = true, float charsDistCoef = 1.0f, 
					  float linesDistCoef = 1.0f);

public:
	/** Returns line height. */
	float getLineHeight() const;

	/** Returns base height. */
	float getBase() const;

	/** Serialize method. */
	SERIALIZE_METHOD_DECL();
};

CLOSE_O2_NAMESPACE

#endif // FONT_H
