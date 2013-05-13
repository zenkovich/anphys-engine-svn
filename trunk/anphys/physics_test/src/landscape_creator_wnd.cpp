#include "landscape_creator_wnd.h"

#include "engine/engine_incl.h"


LandscapeCreatorWnd::LandscapeCreatorWnd( uiWidgetsManager* widgetsManager ):mWidgetsManager(widgetsManager)
{
	resetParametres();

	mWindow = uiSimpleStuff::createWindow(mWidgetsManager, "LanscapeCreatorWnd", vec2(200, 0), vec2(200, 300), 
		"Landscape creator");

	uiSimpleStuff::createSizeEffect(mWindow);

	uiVertLayoutWidget* verLayoutWidget = new uiVertLayoutWidget(mWidgetsManager, "verLayout");
	verLayoutWidget->setPosition(vec2(5, 5));
	verLayoutWidget->mWidgetsDistance = 5.0f;

	uiButton* recreateBtn = uiSimpleStuff::createButton(mWidgetsManager, vec2(0, 0), vec2(100, 25), "landscapeBtn",
		"Recreate", new cCallback<LandscapeCreatorWnd>(this, &LandscapeCreatorWnd::onRecreateLandcapeBtnPressed));

#define pushProperty(value, caption) \
	{ \
		uiHorLayoutWidget* horLayout = new uiHorLayoutWidget(mWidgetsManager, ""); \
		horLayout->mWidgetsDistance = 2; \
		uiLabel* label = uiSimpleStuff::createLabel(mWidgetsManager, vec2(0, 0), vec2(130, 22), "", caption); \
		uiTextEdit* textEdit = uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(100, 0), vec2(50, 22), ""); \
		textEdit->bindValue(&value); \
		horLayout->addChild(label); \
		horLayout->addChild(textEdit); \
		verLayoutWidget->addChild(horLayout); \
	}
	
	pushProperty(mMinRangeX, "Min X Range");
	pushProperty(mMinRangeY, "Min Y Range");
	pushProperty(mMinRangeZ, "Min Z Range");
	
	pushProperty(mMaxRangeX, "Max X Range");
	pushProperty(mMaxRangeY, "Max Y Range");
	pushProperty(mMaxRangeZ, "Max Z Range");
	
	pushProperty(mSegmentsXCount, "Segments X");
	pushProperty(mSegmentsZCount, "Segments Z");
	
	verLayoutWidget->addChild((uiWidget*)recreateBtn);

	mWindow->addChild(verLayoutWidget);

	mWidgetsManager->addWidget(mWindow);
}

LandscapeCreatorWnd::~LandscapeCreatorWnd()
{
	mWidgetsManager->removeWidget(mWindow);
}

void LandscapeCreatorWnd::show()
{
	mWindow->show();
}

void LandscapeCreatorWnd::onRecreateLandcapeBtnPressed()
{

}

void LandscapeCreatorWnd::resetParametres()
{
	mMinRangeX = -100;
	mMinRangeY = -10;
	mMinRangeZ = -100;

	mMaxRangeX = 100;
	mMaxRangeY = 0;
	mMaxRangeZ = 100;

	mSegmentsXCount = 100;
	mSegmentsZCount = 100;
}
