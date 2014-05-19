#ifndef FONT_H
#define FONT_H

#include "public.h"

#include "texture.h"
#include "util/math/rect.h"
#include "util/serialization/serialization.h"

OPEN_O2_NAMESPACE

class grMesh;
class grRenderSystem;

/** Font. Containing array of symbol glyphs, symbol index table. */
class grFont: public cSerializableObj
{
	friend class grFontManager;
	friend class grText;
	friend class shared<grFont>;
	
public:
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

public:
	/** Returns line height. */
	float getLineHeight() const;

	/** Serialize method. */
	SERIALIZE_METHOD_DECL();
};

CLOSE_O2_NAMESPACE

#endif // FONT_H
