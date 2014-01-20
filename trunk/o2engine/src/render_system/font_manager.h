#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <vector>
#include <map>

#include "public.h"

OPEN_O2_NAMESPACE

class grFont;
class grRenderSystem;

class grFontManager
{
	friend class grRenderSystemBaseInterface;

public: 
	typedef std::map<std::string, grFont*> FontsMap;

protected:
	grRenderSystem* mRenderSystem;
	FontsMap        mFonts;
	

	grFontManager(grRenderSystem* renderSystem);
	~grFontManager();

public:
	void loadFontsSet(const std::string& fontsSetFile);
	void loadFont(const std::string& fontFile);
	void loadBMFont(const std::string& fontFile);
	void unloadFonts();

	grFont* getFont(const std::string& name);
};

CLOSE_O2_NAMESPACE

#endif // FONT_MANAGER_H
