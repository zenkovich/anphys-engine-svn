#ifndef UI_SOLID_RECT_H
#define UI_SOLID_RECT_H

#include "ui_widget.h"

struct grSprite;

struct uiSolidRect:public uiWidget
{
	DEFINE_TYPE(uiSolidRect)

	grSprite* mSolidSprite;
	color4    mBordersColor;
	color4    mInColor;

	color4    mResBordersColor;
	color4    mResInColor;

//functions
	uiSolidRect(uiWidgetsManager* widgetsManager, const std::string& id, const color4& bordersColor, 
		        const color4& inColor);
	~uiSolidRect();

	void derivedDraw();
	void derivedUpdate(float dt);

protected:
	void setupInitialProperties();
};

#endif //UI_SOLID_RECT_H