#ifndef FONT_H
#define FONT_H

#include "public.h"

#include "util/math/rect.h"
#include "util/serialization/serialization.h"

OPEN_O2_NAMESPACE

class grTexture;
class grMesh;
class grRenderSystem;

class grFont: public cSerializableObj
{
	friend class grFontManager;
	friend class grText;
	
public:
	struct character: public cSerializableObj
	{
		fRect  mTexSrc;
		vec2f  mSize;
		vec2f  mOffset;
		float  mAdvance;
		uint16 mCharId;

		SERIALIZE_METHOD_DECL();
	};

protected:
	enum { nMaxSymbolId = 0xffff };

	std::string     mName;
	character*      mCharacters;
	int             mCharactersCount;
	bool            mAllSymbolReturn;
	float           mLineHeight;
	float           mBase;
	uint16*         mCharacterIds;
	grTexture*      mTexture;
	grRenderSystem* mRenderSystem;

	
	grFont();
	grFont(const grFont& font);
	~grFont();

	void create(grRenderSystem* renerSystem, const std::string& fontFile);
	void createFromBMFont(grRenderSystem* renderSystem, const std::string& fontFile);

public:
	SERIALIZE_METHOD_DECL();

	float getLineHeight() const;
};

CLOSE_O2_NAMESPACE

#endif // FONT_H
