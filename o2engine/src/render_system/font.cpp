#include "font.h"

#include "render_system.h"
#include "texture.h"
#include "util/xml_tools.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

grFont::grFont():
	mCharacters(NULL), mCharacterIds(NULL), mCharactersCount(0), mTexture(NULL), mRenderSystem(NULL), 
	mAllSymbolReturn(false)
{

}

grFont::grFont( const grFont& font ):
	mCharacters(NULL), mCharacterIds(NULL), mCharactersCount(0), mTexture(NULL), mRenderSystem(NULL)
{
}

grFont::~grFont()
{
	mRenderSystem->releaseTexture(mTexture);
	safe_release_arr(mCharacters);
	safe_release_arr(mCharacterIds);
}

void grFont::create( grRenderSystem* renerSystem, const std::string& fontFile )
{
	mRenderSystem = renerSystem;

	pugi::xml_document doc;
	cXmlTools::loadFromFile(fontFile, doc);
	
	serialize(doc, cSerializeType::INPUT, renerSystem->mLog);
}

void grFont::createFromBMFont( grRenderSystem* renderSystem, const std::string& fontFile )
{
	mRenderSystem = renderSystem;

	pugi::xml_document doc;
	if (!cXmlTools::loadFromFile(fontFile, doc))
	{
		mRenderSystem->mLog->hout("ERROR: Failed to load BMFont file: %s", fontFile.c_str());
		return;
	}

	pugi::xml_node root = doc.child("font");

	if (pugi::xml_node commonNode = root.child("common"))
	{
		mName = commonNode.attribute("name").value();

		std::string textureName = commonNode.attribute("texture").value();		
		mRenderSystem->releaseTexture(mTexture);
		mTexture = mRenderSystem->getTextureFromFile(textureName);

		mLineHeight = commonNode.attribute("lineHeight").as_float();
		mBase = commonNode.attribute("base").as_float();
	}
	else
	{
		mRenderSystem->mLog->hout("ERROR: Failed to get common info in font: %s. Bad file format", fontFile.c_str());
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
		mRenderSystem->mLog->hout("ERROR: Failed to get characters node in BMFont file: %s. Bad file format", fontFile.c_str());
		return;
	}

	vec2f invTexSize(1.0f/mTexture->getSize().x, 1.0f/mTexture->getSize().y);
	for (int i = 0; i < mCharactersCount; i++)
	{
		mCharacters[i].mSize = mCharacters[i].mTexSrc.getSize();
		mCharacters[i].mTexSrc.left *= invTexSize.x;
		mCharacters[i].mTexSrc.right *= invTexSize.x;
		mCharacters[i].mTexSrc.top *= invTexSize.y;
		mCharacters[i].mTexSrc.down *= invTexSize.y;
	}
}

SERIALIZE_METHOD_IMPL(grFont::character) 
{
	SERIALIZE_ID(mTexSrc, "texSrc");	
	SERIALIZE_ID(mOffset.x, "xoffset");
	SERIALIZE_ID(mOffset.y, "yoffset");
	SERIALIZE_ID(mAdvance, "xadvance");
	SERIALIZE_ID(mCharId, "id");

	return true;
}

SERIALIZE_METHOD_IMPL(grFont)
{
	SERIALIZE_ID(mName, "name");

	std::string textureName;
	if (type == cSerializeType::OUTPUT && mTexture)
		textureName = mTexture->getFileName();
	
	SERIALIZE_ID(textureName, "texture");
	SERIALIZE_ID(mLineHeight, "lineHeight");
	SERIALIZE_ID(mBase, "base");
	SERIALIZE_ID(mAllSymbolReturn, "allSymbolsReturn");

	if (type == cSerializeType::INPUT)
	{
		mRenderSystem->releaseTexture(mTexture);
		mTexture = mRenderSystem->getTextureFromFile(textureName);
	}

	SERIALIZE_ID(mCharactersCount, "charactersCount");

	if (type == cSerializeType::INPUT) 
	{
		safe_release_arr(mCharacters);
		safe_release_arr(mCharacterIds);
		mCharacters = mnew character[mCharactersCount];
		mCharacterIds = mnew uint16[nMaxSymbolId];
		memset(mCharacterIds, 0, sizeof(uint16)*nMaxSymbolId);
	}

	for (int i = 0; i < mCharactersCount; i++)
	{
		char id[24]; sprintf(id, "char%", i);
		character* x = &(mCharacters[i]);
		SERIALIZE_ID(x, id);
	}

	if (type == cSerializeType::INPUT) 
	{
		for (int i = 0; i < mCharactersCount; i++)
			mCharacterIds[mCharacters[i].mCharId] = i;
	}

	return true;
}

float grFont::getLineHeight() const
{
	return mCharacters[mCharacterIds['A']].mTexSrc.getSizeY();
}

CLOSE_O2_NAMESPACE