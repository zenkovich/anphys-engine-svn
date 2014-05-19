#include "font_manager.h"

#include "render_system.h"
#include "font.h"

#include "util/xml_tools.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

grFontManager::grFontManager()
{
}

grFontManager::~grFontManager()
{
	unloadFonts();
}

void grFontManager::loadFontsSet( const string& fontsSetFile )
{
	pugi::xml_document doc;
	if (!cXmlTools::loadFromFile(fontsSetFile, doc))
	{
		renderSystem()->mLog->error("Failed to load font set file: %s", fontsSetFile.c_str());
		return;
	}

	for (pugi::xml_node obj = doc.child("Font"); obj; obj = obj.next_sibling("Font"))
	{
		loadFont(obj.attribute("fileName").value());
	}
}

void grFontManager::loadFont( const string& fontFile )
{
	shared<grFont> newFont = mnew grFont;
	newFont->create(fontFile);
	mFonts[fontFile] = newFont;
}

void grFontManager::loadBMFont( const string& fontFile )
{
	shared<grFont> newFont = mnew grFont;
	newFont->createFromBMFont(fontFile);
	mFonts[fontFile] = newFont;
}

void grFontManager::unloadFonts()
{
	for (FontsMap::iterator it = mFonts.begin(); it != mFonts.end(); ++it)
		safe_release(it->second);

	mFonts.clear();
}

shared<grFont> grFontManager::getFont( const string& name )
{
	return mFonts.at(name);
}

CLOSE_O2_NAMESPACE