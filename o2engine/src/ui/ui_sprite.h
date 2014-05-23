#ifndef UI_SPRITE_H
#define UI_SPRITE_H

#include "ui_widget.h"

#include "render_system/sprite.h"

OPEN_O2_NAMESPACE

class uiSprite:public uiWidget
{
public:
	grSprite mSprite;

	uiSprite(const uiWidgetLayout& layout, const string& id = "", shared<uiWidget> parent = NULL);

	/** copy-ctor. */
	uiSprite(const uiSprite& spriteWidget);

	/** dtor. */
	~uiSprite();

	/** Returns clone of widget. */
	shared<uiWidget> clone() const;

private:
	/** Drawing current widget. */
	void localDraw();

	void layoutUpdated();
};

CLOSE_O2_NAMESPACE

#endif //UI_SPRITE_H
