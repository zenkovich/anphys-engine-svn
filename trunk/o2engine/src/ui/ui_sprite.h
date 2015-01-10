#ifndef UI_SPRITE_H
#define UI_SPRITE_H

#include "ui_widget.h"

#include "render_system/sprite.h"

OPEN_O2_NAMESPACE

/** UI sprite widget. Just draing a sprite. */
class uiSprite:public uiWidget
{
public:
	grSprite mSprite; /** Sprite drawable. */


	/** ctor. */
	uiSprite(const layout& layout, const string& id = "");

	/** copy-ctor. */
	uiSprite(const uiSprite& spriteWidget);

	/** dtor. */
	~uiSprite();

	/** Returns clone of widget. */
	uiWidget* clone() const;

private:
	/** Drawing current widget. */
	void localDraw();

	/** Calls when widget layout was updated. */
	void layoutUpdated();

	/** Calls when parameter "transparency" was changed. */
	void transparencyChanged();
};

CLOSE_O2_NAMESPACE

#endif //UI_SPRITE_H
