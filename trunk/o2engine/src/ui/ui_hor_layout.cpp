#include "ui_hor_layout.h"

OPEN_O2_NAMESPACE

uiHorLayout::uiHorLayout( const cLayout& layout, const string& id /*= ""*/ ):
	uiWidget(layout, id)
{
}

uiHorLayout::uiHorLayout( const uiHorLayout& widget ):
	uiWidget(widget)
{
}

uiHorLayout::~uiHorLayout()
{
}

uiWidget* uiHorLayout::clone() const
{
	return mnew uiHorLayout(*this);
}

void uiHorLayout::layoutUpdated()
{
	float childsRelSize = 1.0f/(float)mChildWidgets.size();

	float xRelOffset = 0.0f;
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		cLayout lt = (*child)->getLayout();
		lt.mLTAbsolute.x = 0.0f;
		lt.mLTRelative.x = xRelOffset;

		xRelOffset += childsRelSize;

		lt.mRBAbsolute.x = 0.0f;
		lt.mRBRelative.x = xRelOffset;

		(*child)->setLayout(lt);
	}
}

CLOSE_O2_NAMESPACE