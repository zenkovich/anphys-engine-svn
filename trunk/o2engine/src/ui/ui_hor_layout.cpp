#include "ui_hor_layout.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiHorLayout);

uiHorLayout::uiHorLayout( const cLayout& layout, float widgetsDistance /*= 10.0f*/, const string& id /*= ""*/ ):
	uiWidget(layout, id), mWidgetsDistance(widgetsDistance)
{
}

uiHorLayout::uiHorLayout( const uiHorLayout& widget ):
	uiWidget(widget)
{
	mWidgetsDistance = widget.mWidgetsDistance;
}

uiHorLayout::~uiHorLayout()
{
}

uiWidget* uiHorLayout::clone() const
{
	return mnew uiHorLayout(*this);
}

void uiHorLayout::setWidgetsDistance(float distance)
{
	mWidgetsDistance = distance;
	updateLayout();
}

float uiHorLayout::getWidgetsDistance() const
{
	return mWidgetsDistance;
}

void uiHorLayout::layoutUpdated()
{
	float childsLayoutSize = mChildsLayout.mSize.x - mWidgetsDistance*(float)(mChildWidgets.size() - 1);
	float childSize = childsLayoutSize/(float)mChildWidgets.size();
	
	mFlexibleWidgets.clear();
	float offset = 0.0f;
	float addOffset = 0.0f;
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		cLayout lt = (*child)->getLayout();

		lt.mLTAbsolute.x = offset;             lt.mLTRelative.x = 0.0f;
		lt.mRBAbsolute.x = offset + childSize; lt.mRBRelative.x = 0.0f;

		(*child)->setLayout(lt);
		lt = (*child)->getLayout();

		offset = lt.right() - mChildsLayout.left() + mWidgetsDistance;

		float d = childSize - lt.mSize.x;
		addOffset += d;
		if (equals(d, 0.0f, 1.0f))
			mFlexibleWidgets.push_back(*child);
	}

	addOffset /= (float)mFlexibleWidgets.size();

	offset = 0.0f;
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		cLayout lt = (*child)->getLayout();

		lt.mLTAbsolute.x = offset;             lt.mLTRelative.x = 0.0f;
		lt.mRBAbsolute.x = offset + childSize; lt.mRBRelative.x = 0.0f;

		if (FIND(mFlexibleWidgets, *child) != mFlexibleWidgets.end())
			lt.mRBAbsolute.x += addOffset;

		(*child)->setLayout(lt);
		lt = (*child)->getLayout();

		offset = lt.right() - mChildsLayout.left() + mWidgetsDistance;
	}
}

CLOSE_O2_NAMESPACE