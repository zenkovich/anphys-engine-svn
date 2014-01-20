#include "font_manager.h"

#include "render_system.h"
#include "font.h"

#include "util/xml_tools.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

grFontManager::grFontManager( grRenderSystem* renderSystem ):
	mRenderSystem(renderSystem)
{
}

grFontManager::~grFontManager()
{
	unloadFonts();
}

void grFontManager::loadFontsSet( const std::string& fontsSetFile )
{
	pugi::xml_document doc;
	if (!cXmlTools::loadFromFile(fontsSetFile, doc))
	{
		mRenderSystem->mLog->hout("ERROR: Failed to load font set file: %s", fontsSetFile.c_str());
		return;
	}

	for (pugi::xml_node obj = doc.child("Font"); obj; obj = obj.next_sibling("Font"))
	{
		loadFont(obj.attribute("fileName").value());
	}
}

void grFontManager::loadFont( const std::string& fontFile )
{
	grFont* newFont = mnew grFont;
	newFont->create(mRenderSystem, fontFile);
	mFonts[fontFile] = newFont;
}

void grFontManager::loadBMFont( const std::string& fontFile )
{
	grFont* newFont = mnew grFont;
	newFont->createFromBMFont(mRenderSystem, fontFile);
	mFonts[fontFile] = newFont;
}

void grFontManager::unloadFonts()
{
	for (FontsMap::iterator it = mFonts.begin(); it != mFonts.end(); ++it)
		safe_release(it->second);

	mFonts.clear();
}

grFont* grFontManager::getFont( const std::string& name )
{
	return mFonts.at(name);
}

CLOSE_O2_NAMESPACE