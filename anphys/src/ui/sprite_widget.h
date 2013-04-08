#ifndef SPRITE_WIDGET_H
#define SPRITE_WIDGET_H

#include "widget.h"

struct grSprite;

struct uiSpriteWidget:public uiWidget
{
	DEFINE_TYPE(uiSpriteWidget)

	grSprite* mSprite;
	bool      mOwnSprite;

//functions
	uiSpriteWidget(grSprite* sprite = NULL, bool ownSprite = true, const std::string& id = "noName", uiWidget* parent = NULL);
	uiSpriteWidget(const std::string& spriteSrcFile, const std::string& spriteSrcObjectpath,
		           const std::string& id = "noName", uiWidget* parent = NULL);
	uiSpriteWidget(cDataObject* dataObject, uiWidget* parent = NULL);
	uiSpriteWidget(const uiSpriteWidget& spriteWidget);
	~uiSpriteWidget();

	virtual void update(float dt);
	virtual void draw();

	virtual uiWidget* clone() const;
};

#endif //SPRITE_WIDGET_H