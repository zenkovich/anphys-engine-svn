#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <map>

#include "public.h"

OPEN_O2_NAMESPACE

class grFont;
class grRenderSystem;

/** Font manager. Containing set of fonts, loading/unloading fonts. */
class grFontManager: public cShareObject
{
	friend class grRenderSystemBaseInterface;
	friend class shared<grFontManager>;

public: 
	typedef std::map< string, shared<grFont> > FontsMap; 

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
	shared<grFont> loadFont(const string& fontFile);

	/** Loading BMFont from file. */
	shared<grFont> loadBMFont(const string& fontFile);

	/** Unloading all fonts. */
	void unloadFonts();

	/** Returns font by name. */
	shared<grFont> getFont(const string& name);
};

CLOSE_O2_NAMESPACE

#endif // FONT_MANAGER_H
