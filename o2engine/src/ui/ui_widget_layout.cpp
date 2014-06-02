#include "ui_widget_layout.h"

OPEN_O2_NAMESPACE
	
uiWidgetLayout::uiWidgetLayout( const vec2f& relPosition /*= vec2f()*/, const vec2f& pxPosition /*= vec2f()*/, 
	                            const vec2f& minSize /*= vec2f()*/, const vec2f& maxSize /*= vec2f()*/, 
					            const vec2f& relSize /*= vec2f()*/, const vec2f& pxSize /*= vec2f()*/, 
					            const vec2f& relPivot /*= vec2f()*/, const vec2f& pxPivot /*= vec2f()*/ ):
	mRelPosition(relPosition), mPxPosition(pxPosition), mMinSize(minSize), mMaxSize(maxSize), mRelSize(relSize),
	mPxSize(pxSize), mRelPivot(relPivot), mPxPivot(pxPivot)
{
}

uiWidgetLayout::uiWidgetLayout( const uiWidgetLayout& layout )
{
	mRelPosition = layout.mRelPosition;
	mPxPosition = layout.mPxPosition;
	mRelPivot = layout.mRelPivot;
	mPxPivot = layout.mPxPivot;
	mMinSize = layout.mMinSize;
	mMaxSize = layout.mMaxSize;
	mRelSize = layout.mRelSize;
	mPxSize = layout.mPxSize;
}

void uiWidgetLayout::calculate( const vec2f& iniPosition, const vec2f& iniSize, vec2f& position, vec2f& size )
{
	size.x = clamp(iniSize.x*mRelSize.x + mPxSize.x, mMinSize.x, mMaxSize.x);
	size.y = clamp(iniSize.y*mRelSize.y + mPxSize.y, mMinSize.y, mMaxSize.y);

	vec2f pivot = size.scale(mRelPivot) + mPxPivot;

	position = iniPosition + iniSize.scale(mRelPosition) - pivot + mPxPosition;
}

uiWidgetLayout uiStraightPixelLayout( const vec2f& position, const vec2f& size )
{
	return uiWidgetLayout(vec2f(), position, size);
}

uiWidgetLayout uiBothLayout( const fRect& border /*= fRect()*/ )
{
	return uiWidgetLayout(vec2f(), vec2f(border.left, border.right), vec2f(), vec2f(FLT_MAX, FLT_MAX), vec2f(1, 1),
		                  vec2f(-border.left - border.right, -border.top - border.down));
}

CLOSE_O2_NAMESPACE