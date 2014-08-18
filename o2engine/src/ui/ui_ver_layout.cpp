#include "ui_ver_layout.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiVerLayout);

uiVerLayout::uiVerLayout( const cLayout& layout, float widgetsDistance /*= 10.0f*/, const string& id /*= ""*/ ):
	uiWidget(layout, id), mWidgetsDistance(widgetsDistance)
{
}

uiVerLayout::uiVerLayout( const uiVerLayout& widget ):
	uiWidget(widget)
{
	mWidgetsDistance = widget.mWidgetsDistance;
}

uiVerLayout::~uiVerLayout()
{
}

uiWidget* uiVerLayout::clone() const
{
	return mnew uiVerLayout(*this);
}

void uiVerLayout::setWidgetsDistance(float distance)
{
	mWidgetsDistance = distance;
	updateLayout();
}

float uiVerLayout::getWidgetsDistance() const
{
	return mWidgetsDistance;
}

void uiVerLayout::layoutUpdated()
{
	float childsLayoutSize = mChildsLayout.mSize.y - mWidgetsDistance*(float)(mChildWidgets.size() - 1);
	float childSize = childsLayoutSize/(float)mChildWidgets.size();
	
	mFlexibleWidgets.clear();
	float offset = 0.0f;
	float addOffset = 0.0f;
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		cLayout lt = (*child)->getLayout();

		lt.mLTAbsolute.y = offset;             lt.mLTRelative.y = 0.0f;
		lt.mRBAbsolute.y = offset + childSize; lt.mRBRelative.y = 0.0f;

		(*child)->setLayoutParams(lt, true);
		lt = (*child)->getLayout();

		offset = lt.down() - mChildsLayout.top() + mWidgetsDistance;

		float d = childSize - lt.mSize.y;
		addOffset += d;
		if (equals(d, 0.0f, 1.0f))
			mFlexibleWidgets.push_back(*child);
	}

	addOffset /= (float)mFlexibleWidgets.size();

	offset = 0.0f;
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		cLayout lt = (*child)->getLayout();

		lt.mLTAbsolute.y = offset;             lt.mLTRelative.y = 0.0f;
		lt.mRBAbsolute.y = offset + childSize; lt.mRBRelative.y = 0.0f;

		if (FIND(mFlexibleWidgets, *child) != mFlexibleWidgets.end())
			lt.mRBAbsolute.y += addOffset;

		(*child)->setLayoutParams(lt, true);
		lt = (*child)->getLayout();

		offset = lt.down() - mChildsLayout.top() + mWidgetsDistance;
	}
}

uiWidget* uiVerLayout::addChild( uiWidget* widget, int position /*= -1*/ )
{
	uiWidget* res = uiWidget::addChild(widget, position);
	updateLayout(true);
	return res;
}

CLOSE_O2_NAMESPACE