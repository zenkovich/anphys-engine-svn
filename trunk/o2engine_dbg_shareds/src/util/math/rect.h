#ifndef RECT_H
#define RECT_H

#include <float.h>

#include "util/public_namespace.h"
#include "vector2.h"
#include "math.h"

OPEN_O2_NAMESPACE

template<typename T>
struct Rect
{
	T left, top, right, down;

	inline Rect() 
	{ 
		left = top = right = down = 0;
	}

	inline Rect(const vec2<T>& leftTop, const vec2<T>& rightDown) 
	{ 
		left = leftTop.x; top = leftTop.y;
		right = rightDown.x; down = rightDown.y;
	}

	inline Rect(float vleft, float vtop, float vright, float vbottom)
	{
		left = vleft; top = vtop;
		right = vright; down = vbottom;
	}

	inline Rect(const Rect<T>& rect)
	{
		left = rect.left; top = rect.top;
		right = rect.right; down = rect.down;
	}

	inline bool operator==(const Rect<T>& rt) const
	{
		if (abs(left - rt.left) > (T)FLT_EPSILON || abs(top - rt.top) > (T)FLT_EPSILON ||
            abs(right - rt.right) > (T)FLT_EPSILON || abs(down - rt.down) > (T)FLT_EPSILON)
			return false;

		return true;
	}

	inline bool operator!=(const Rect<T>& rt) const
	{
		return !(*this == rt);
	}

	inline Rect<T>& operator=(const Rect<T>& rect)
	{
		left = rect.left; top = rect.top;
		right = rect.right; down = rect.down;
		return *this;
	}

	inline Rect<T> operator+(const vec2<T>& v) const
	{ 
		return Rect<T>(left + v.x, top + v.y, right + v.x, down + v.y);
	}

	inline Rect<T> operator*(const vec2<T>& v) const
	{ 
		return Rect<T>(left*v.x, top*v.y, right*v.x, down*v.y);
	}

	inline Rect<T> operator/(const vec2<T>& v) const
	{ 
		return Rect<T>(left/v.x, top/v.y, right/v.x, down/v.y);
	}

	inline Rect<T> operator+(const Rect<T>& r) const
	{ 
		return Rect<T>(min(left, r.left), min(top, r.top), max(right, r.right), max(down, r.down));
	}

	inline void set(float vleft, float vtop, float vright, float vbottom)
	{
		left = vleft; top = vtop;
		right = vright; down = vbottom;
	}

	inline void set(const vec2f& leftTop, const vec2f& rightDown)
	{
		left = leftTop.x; top = leftTop.y;
		right = rightDown.x; down = rightDown.y;
	}

	inline vec2<T> getSize() const
	{
		return vec2<T>(getSizeX(), getSizeY()); 
	}

	inline T getSizeX() const
	{ 
		return abs(right - left);
	}

	inline T getSizeY() const
	{
		return abs(down - top);
	}

	inline vec2<T> getltCorner() const
	{
		return vec2<T>(left, top); 
	}

	inline vec2<T> getrtCorner() const
	{ 
		return vec2<T>(right, top); 
	}

	inline vec2<T> getldCorner() const
	{ 
		return vec2<T>(left, down);
	}

	inline vec2<T> getrdCorner() const
	{ 
		return vec2<T>(right, down);; 
	}

	inline bool isInside(const vec2<T>& p) const
	{
		if (p.x < left || p.x > right ||
			p.y < top  || p.y > down)
		{
			return false;
		}

		return true;
	}

	inline bool isZero() const 
	{
		return (left == 0.0f && right == 0.0f && top == 0.0f && down == 0.0f);
	}
};

#define fRect Rect<float>
#define iRect Rect<int>

CLOSE_O2_NAMESPACE

#endif //RECT_H