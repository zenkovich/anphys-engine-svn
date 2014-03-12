#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <vector>
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
	typedef std::map<std::string, grFont*> FontsMap; 

protected:
	FontsMap mFonts; /** Fonts map. */
	
	/** ctor. */
	grFontManager();

	/** dtor. */
	~grFontManager();

public:
	/** Loading fonts set from file. */
	void loadFontsSet(const std::string& fontsSetFile);

	/** Loading font from file. */
	void loadFont(const std::string& fontFile);

	/** Loading BMFont from file. */
	void loadBMFont(const std::string& fontFile);

	/** Unloading all fonts. */
	void unloadFonts();

	/** Returns font by name. */
	grFont* getFont(const std::string& name);
};

CLOSE_O2_NAMESPACE

#endif // FONT_MANAGER_H
