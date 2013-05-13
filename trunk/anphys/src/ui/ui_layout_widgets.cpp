#include "ui_layout_widgets.h"

REGIST_TYPE(uiHorLayoutWidget)
REGIST_TYPE(uiVertLayoutWidget)

uiHorLayoutWidget::uiHorLayoutWidget( uiWidgetsManager* widgetsManager, const std::string& id ):
	uiWidget(widgetsManager, id), mCurrentWidth(0), mWidgetsDistance(5)
{
}

void uiHorLayoutWidget::addChild( uiWidget* widget )
{
	uiWidget::addChild(widget);
	widget->setPosition(vec2(mCurrentWidth, 0));
	mCurrentWidth += widget->getSize().x + mWidgetsDistance;

	adjustSizeByChilds();
}


uiVertLayoutWidget::uiVertLayoutWidget( uiWidgetsManager* widgetsManager, const std::string& id ):
	uiWidget(widgetsManager, id), mCurrentHeight(0), mWidgetsDistance(5)
{
}

void uiVertLayoutWidget::addChild( uiWidget* widget )
{
	uiWidget::addChild(widget);
	widget->setPosition(widget->getPosition() + vec2(0, mCurrentHeight));
	mCurrentHeight += widget->getSize().y + mWidgetsDistance;

	adjustSizeByChilds();
}
