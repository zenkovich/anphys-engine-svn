#ifndef SPRITE_WIDGET_H
#define SPRITE_WIDGET_H

#include "ui_widget.h"

#include "util/math/color.h"

struct grSprite;

struct uiSpriteWidget:public uiWidget
{
	DEFINE_TYPE(uiSpriteWidget)

	grSprite*   mSprite;
	bool        mOwnSprite;
	color4      mSpriteColor;
	color4      mInitialSpriteColor;

	std::string mSpriteSrcFile;
	std::string mSpriteSrcObjectPath;

//functions
	uiSpriteWidget(uiWidgetsManager* widgetsManager, grSprite* sprite = NULL, bool ownSprite = true, 
		           const std::string& id = "noName");

	uiSpriteWidget(uiWidgetsManager* widgetsManager, 
		           const std::string& spriteSrcFile, const std::string& spriteSrcObjectpath,
		           const std::string& id = "noName");

	uiSpriteWidget(uiWidgetsManager* widgetsManager, cDataObject* dataObject);

	uiSpriteWidget(const uiSpriteWidget& spriteWidget);
	~uiSpriteWidget();

	virtual void derivedUpdate(float dt);
	virtual void derivedDraw();

	uiSpriteWidget* setSize(const vec2& size);

	virtual uiWidget* clone() const;

	bool isAdjustingSizeByChilds() { return false; }

	derivedSerializeMethodDecl(uiWidget, uiSpriteWidget);

protected:
	void setupInitialProperties();
};

#endif //SPRITE_WIDGET_H