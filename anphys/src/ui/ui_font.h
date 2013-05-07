#ifndef UI_FONT_H
#define UI_FONT_H

#include <string>
#include <vector>

#include "util/math/rect.h"
#include "util/math/color.h"
#include "util/types.h"

struct grRender2DObjectMesh;
struct cDataObject;
struct grRender;

struct uiFont
{
	typedef std::vector<fRect> RectsList;
	typedef std::vector<unsigned int> IdList;

	struct StrLineCache
	{
		struct Character
		{
			char16_t     mCharacter;
			fRect        mGeometry;
			fRect        mTextureCoords;

			Character() {}
			Character(char16_t character, const fRect& geometry, const fRect& textureCoords):
				mCharacter(character), mGeometry(geometry), mTextureCoords(textureCoords) {}

			Character(const Character& charc)
			{
				mCharacter     = charc.mCharacter;
				mGeometry      = charc.mGeometry;
				mTextureCoords = charc.mTextureCoords;
			}
		};
		typedef std::vector<Character> CharactersList;

		fRect          mRect;

		CharactersList mCharacters;

		unsigned int   mStartSymbol;
		unsigned int   mEndSymbol;
		unsigned int   mSpacesCount;

		wstring        mStr;

		void pushCharacter(const Character& charc, int symbolIdx, bool isSpace, float diffCoef);
		void reset(unsigned int startSymbol);
	};
	typedef std::vector<StrLineCache> CacheLinesList;

	enum HorAlign { AL_LEFT = 0, AL_CENTER, AL_RIGHT, AL_WIDEH };
	enum VerAlign { AL_TOP = 0, AL_MIDDLE, AL_BOTTOM, AL_WIDEV };

	enum { nMaxSymbols = 2048, nMaxCharId = 512 };

protected:
	RectsList             mCharacters;
	unsigned int          mCharactedIdList[nMaxCharId];

	grRender2DObjectMesh* mMesh;

	wstring               mText;
	fRect                 mTextArea;
	fRect                 mClippingArea;
	bool                  mClipping;
	bool                  mWordWrap;
	HorAlign              mHorAlign;
	VerAlign              mVerAlign;
	vec2                  mDistCoef;
	vec2                  mScale;
	color4                mColor;
				          
	bool                  mNeedUpdateMesh;
				          
	grRender*             mRender;

public:
	fRect                 mRealTextRect;
	CacheLinesList        mCachedLines;

	uiFont(grRender* render);
	uiFont(const uiFont& font);
	~uiFont();

	void         load(cDataObject* dataObject);
	void         load(const std::string& file, const std::string& objectPath);
	void         loadWelloreFormat(const std::string& file);

	uiFont&      setText(const std::string& text);
	std::string  getText();

	uiFont&      setText(const wstring& text);
	wstring&     getWText(bool textWillChange = true);

	uiFont&      setHorAlign(HorAlign align);
	HorAlign     getHorAlign() const;

	uiFont&      setVerAlign(VerAlign align);
	VerAlign     getVerAlign() const;

	uiFont&      setPosition(const vec2& position);
	vec2         getPosition();

	uiFont&      setTextAreaSize(const vec2& size);

	uiFont&      setTextArea(const fRect& rect);
	fRect        getTextArea() const;

	uiFont&      setClippingArea(const fRect& rect);
	fRect        getClippingArea() const;
	uiFont&      setClipping(bool flag);
	bool         isClipping() const;

	uiFont&      setDistCoef(const vec2& coef);
	vec2         getDistCoef() const;

	uiFont&      setScale(const vec2& scale);
	vec2         getScale() const;

	uiFont&      setColor(const color4& color);
	color4       getColor() const;

	uiFont&      setWordWrap(bool wordWrap);
	bool         isWordWrap() const;

	void         draw();

	uiFont*      clone();

protected:		 
	void         updateMesh();
	void         createMesh();
};

#endif //UI_FONT_H