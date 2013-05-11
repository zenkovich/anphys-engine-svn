#ifndef UI_BORDER_H
#define UI_BORDER_H

#include "ui_widget.h"

struct uiFont;

struct uiBorder:public uiWidget
{
	DEFINE_TYPE(uiBorder)

	enum LineType { LT_NO_LINE = 0, LT_SINGLE, LT_DOUBLE };

	fRect    mBorderSizes;
	uiFont*  mCaption;
	LineType mLinesType;
	color4   mLinesColor;

//functions
	uiBorder(uiWidgetsManager* widgetsManager, const std::string& id, uiFont* captionFont, 
		     const fRect& borderSizes = fRect(5, 5, 5, 5), LineType linesType = LT_NO_LINE, 
			 const color4& linesColor = color4(150, 150, 150, 255));
	~uiBorder();

	void derivedUpdate(float dt);
	void derivedDraw();

	void addChild(uiWidget* widget);

	bool isAdjustingSizeByChilds() { return false; }
};

#endif //UI_BORDER_H