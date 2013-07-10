#ifndef RECT_H
#define RECT_H

#include "vector2.h"

template<typename T>
struct Rect
{
	vec2<T> leftTop, rightDown;

	inline Rect() 
	{ 
		leftTop = rightDown = vec2<T>(0, 0); 
	}

	inline Rect(const vec2<T>& lt, const vec2<T>& rd) 
	{ 
		leftTop = lt; rightDown = rd;
	}

	inline Rect(float left, float top, float right, float bottom)
	{
		leftTop = vec2<T>(left, top);
		rightDown = vec2<T>(right, bottom);
	}

	inline Rect(const Rect<T>& rect)
	{
		leftTop = rect.leftTop;
		rightDown = rect.rightDown;
	}

	inline bool operator==(const Rect<T>& rt) const
	{
		if (abs(leftTop.x - rt.leftTop.x) > (T)FLT_EPSILON || abs(leftTop.y - rt.leftTop.y) > (T)FLT_EPSILON ||
            abs(rightDown.x - rt.rightDown.x) > (T)FLT_EPSILON || abs(rightDown.y - rt.rightDown.y) > (T)FLT_EPSILON)
			return false;

		return true;
	}

	inline bool operator!=(const Rect<T>& rt) const
	{
		return !(*this == rt);
	}

	inline Rect<T>& operator=(const Rect<T>& rt)
	{
		leftTop = rt.leftTop;
		rightDown = rt.rightDown;
		return *this;
	}

	inline Rect<T> operator+(const vec2<T>& v) const
	{ 
		return Rect<T>(leftTop + v, rightDown + v);
	}

	inline vec2<T> getSize() const
	{
		return vec2<T>(getSizeX(), getSizeY()); 
	}

	inline T getSizeX() const
	{ 
		return abs(rightDown.x - leftTop.x);
	}

	inline T getSizeY() const
	{
		return abs(rightDown.y - leftTop.y);
	}

	inline vec2<T> getltCorner() const
	{
		return leftTop; 
	}

	inline vec2<T> getrtCorner() const
	{ 
		return vec2<T>(rightDown.x, leftTop.y); 
	}

	inline vec2<T> getldCorner() const
	{ 
		return vec2<T>(leftTop.x, rightDown.y);
	}

	inline vec2<T> getrdCorner() const
	{ 
		return rightDown; 
	}

	inline bool inRect(const vec2<T>& p) const
	{
		if (p.x < leftTop.x || p.x > rightDown.x ||
			p.y < leftTop.y || p.y > rightDown.y)
		{
			return false;
		}

		return true;
	}
};

#define fRect Rect<float>
#define iRect Rect<int>

#endif //RECT_H