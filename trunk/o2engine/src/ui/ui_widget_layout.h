#ifndef UI_WIDGET_LAYOUT_H
#define UI_WIDGET_LAYOUT_H

#include <float.h>

#include "public.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

struct uiWidgetLayout
{
	vec2f mRelPosition;
	vec2f mPxPosition;
	vec2f mRelPivot;
	vec2f mPxPivot;
	vec2f mMinSize;
	vec2f mMaxSize;
	vec2f mRelSize;
	vec2f mPxSize;

	uiWidgetLayout(const vec2f& pxPosition = vec2f(), const vec2f& minSize = vec2f(), 
		           const vec2f& relPosition = vec2f(), const vec2f& maxSize = vec2f(FLT_MAX, FLT_MAX), 
			       const vec2f& relSize = vec2f(), const vec2f& pxSize = vec2f(),
			       const vec2f& relPivot = vec2f(), const vec2f& pxPivot = vec2f());

	uiWidgetLayout(const uiWidgetLayout& layout);

	void calculate(const vec2f& iniPosition, const vec2f& iniSize, vec2f& position, vec2f& size);
};

uiWidgetLayout uiStraightPixelLayout(const vec2f& position, const vec2f& size);

uiWidgetLayout uiBothLayout(const fRect& border = fRect());

CLOSE_O2_NAMESPACE

#endif // UI_WIDGET_LAYOUT_H
