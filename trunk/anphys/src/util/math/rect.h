#ifndef RECT_H
#define RECT_H

#include "engine/engine_options.h"

#ifdef PLATFORM_WIN32
#include <windows.h>
#endif

#include <math.h>

#include "vec2.h"

struct fRect
{
	vec2 leftTop, rightDown;

	inline fRect() 
	{ 
		leftTop = rightDown = vec2(0, 0); 
	}

	inline fRect(vec2 lt, vec2 rd) 
	{ 
		leftTop = lt; rightDown = rd;
	}

	inline fRect(float left, float top, float right, float bottom)
	{
		leftTop = vec2(left, top);
		rightDown = vec2(right, bottom);
	}

	inline fRect(const fRect& rect)
	{
		leftTop = rect.leftTop;
		rightDown = rect.rightDown;
	}

	inline bool operator==(const fRect& rt)
	{
		if (fabs(leftTop.x - rt.leftTop.x) > FLT_EPSILON || fabs(leftTop.y - rt.leftTop.y) > FLT_EPSILON ||
            fabs(rightDown.x - rt.rightDown.x) > FLT_EPSILON || fabs(rightDown.y - rt.rightDown.y) > FLT_EPSILON)
			return false;

		return true;
	}

	inline bool operator!=(const fRect& rt)
	{
		return !(*this == rt);
	}

	inline fRect& operator=(const fRect& rt)
	{
		leftTop = rt.leftTop;
		rightDown = rt.rightDown;
		return *this;
	}

	inline vec2 getSize() const
	{
		return vec2(getSizeX(), getSizeY()); 
	}

	inline float getSizeX() const
	{ 
		return abs(rightDown.x - leftTop.x);
	}

	inline float getSizeY() const
	{
		return abs(rightDown.y - leftTop.y);
	}

	inline vec2 getltCorner() const
	{
		return leftTop; 
	}

	inline vec2 getrtCorner() const
	{ 
		return vec2(rightDown.x, leftTop.y); 
	}

	inline vec2 getldCorner() const
	{ 
		return vec2(leftTop.x, rightDown.y);
	}

	inline vec2 getrdCorner() const
	{ 
		return rightDown; 
	}

	inline bool inRect(const vec2& p) const
	{
		if (p.x < leftTop.x || p.x > rightDown.x ||
			p.y < leftTop.y || p.y > rightDown.y) return false;
		return true;
	}

	inline fRect plusVector(const vec2& v) const 
	{
		return fRect(leftTop + v, rightDown + v);
	}


#ifdef PLATFORM_WIN32
	inline fRect(RECT rt)
	{
		leftTop = vec2((float)rt.left, (float)rt.top);
		rightDown = vec2((float)rt.right, (float)rt.bottom);
	}

/*	inline void operator=(RECT rt)
	{
		leftTop = vec2((float)rt.left, (float)rt.top);
		rightDown = vec2((float)rt.right, (float)rt.bottom);
	}*/

	inline RECT getWinRect() const
	{
		RECT rt = { (int)leftTop.x, (int)leftTop.y, (int)rightDown.x, (int)rightDown.y };
		return rt;
	}
#endif
};

#endif //RECT_H