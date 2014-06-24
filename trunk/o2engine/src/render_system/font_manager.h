#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <map>

#include "public.h"

OPEN_O2_NAMESPACE

class grFont;
class grRenderSystem;

/** Font manager. Containing set of fonts, loading/unloading fonts. */
class grFontManager
{
	friend class grRenderSystemBaseInterface;

public: 
	typedef std::map< string, grFont* > FontsMap; 

protected:
	FontsMap mFonts; /** Fonts map. */
	
	/** ctor. */
	grFontManager();

	/** dtor. */
	~grFontManager();

public:
	/** Loading fonts set from file. */
	void loadFontsSet(const string& fontsSetFile);

	/** Loading font from file. */
	grFont* loadFont(const string& fontFile);

	/** Loading BMFont from file. */
	grFont* loadBMFont(const string& fontFile);

	/** Unloading all fonts. */
	void unloadFonts();

	/** Returns font by name. */
	grFont* getFont(const string& name);
};

CLOSE_O2_NAMESPACE

#endif // FONT_MANAGER_H
