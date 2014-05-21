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

uiWidgetLayout uiStaightPixelLayout( const vec2f& position, const vec2f& size )
{
	return uiWidgetLayout(vec2f(), position, size);
}

uiWidgetLayout uiBothLayout( const vec2f& border )
{
	return uiWidgetLayout(vec2f(), vec2f(), vec2f(), vec2f(FLT_MAX, FLT_MAX), vec2f(1, 1));
}

CLOSE_O2_NAMESPACE