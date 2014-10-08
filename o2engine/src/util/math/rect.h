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

public:
	Rect();
	Rect(const vec2<T>& leftTop, const vec2<T>& rightDown);
	Rect(float vleft, float vtop, float vright, float vbottom);
	Rect(const Rect<T>& rect);

	bool operator==(const Rect<T>& rt) const;
	bool operator!=(const Rect<T>& rt) const;

	Rect<T>& operator=(const Rect<T>& rect);

	Rect<T> operator+(const vec2<T>& v) const;
	Rect<T> operator-(const vec2<T>& v) const;
	Rect<T> operator*(const vec2<T>& v) const;
	Rect<T> operator/(const vec2<T>& v) const;
	Rect<T> operator+(const Rect<T>& r) const;

	void set(float vleft, float vtop, float vright, float vbottom);
	void set(const vec2f& leftTop, const vec2f& rightDown);

	vec2<T> getSize() const;
	T getSizeX() const;
	T getSizeY() const;

	vec2<T> getltCorner() const;
	vec2<T> getrtCorner() const;
	vec2<T> getldCorner() const;
	vec2<T> getrdCorner() const;

	bool isInside(const vec2<T>& p) const;
	bool isZero() const;
};

//implementation
template<typename T>
Rect<T>::Rect() 
{ 
	left = top = right = down = 0;
}

template<typename T>
Rect<T>::Rect(const vec2<T>& leftTop, const vec2<T>& rightDown) 
{ 
	left = leftTop.x; top = leftTop.y;
	right = rightDown.x; down = rightDown.y;
}

template<typename T>
Rect<T>::Rect(float vleft, float vtop, float vright, float vbottom)
{
	left = vleft; top = vtop;
	right = vright; down = vbottom;
}

template<typename T>
Rect<T>::Rect(const Rect<T>& rect)
{
	left = rect.left; top = rect.top;
	right = rect.right; down = rect.down;
}

template<typename T>
bool Rect<T>::operator==(const Rect<T>& rt) const
{
	if (abs(left - rt.left) > (T)FLT_EPSILON || abs(top - rt.top) > (T)FLT_EPSILON ||
        abs(right - rt.right) > (T)FLT_EPSILON || abs(down - rt.down) > (T)FLT_EPSILON)
		return false;

	return true;
}

template<typename T>
bool Rect<T>::operator!=(const Rect<T>& rt) const
{
	return !(*this == rt);
}

template<typename T>
Rect<T>& Rect<T>::operator=(const Rect<T>& rect)
{
	left = rect.left; top = rect.top;
	right = rect.right; down = rect.down;
	return *this;
}

template<typename T>
Rect<T> Rect<T>::operator+(const vec2<T>& v) const
{ 
	return Rect<T>(left + v.x, top + v.y, right + v.x, down + v.y);
}

template<typename T>
Rect<T> Rect<T>::operator-(const vec2<T>& v) const
{ 
	return Rect<T>(left - v.x, top - v.y, right - v.x, down - v.y);
}

template<typename T>
Rect<T> Rect<T>::operator*(const vec2<T>& v) const
{ 
	return Rect<T>(left*v.x, top*v.y, right*v.x, down*v.y);
}

template<typename T>
Rect<T> Rect<T>::operator/(const vec2<T>& v) const
{ 
	return Rect<T>(left/v.x, top/v.y, right/v.x, down/v.y);
}

template<typename T>
Rect<T> Rect<T>::operator+(const Rect<T>& r) const
{ 
	return Rect<T>(min(left, r.left), min(top, r.top), max(right, r.right), max(down, r.down));
}

template<typename T>
void Rect<T>::set(float vleft, float vtop, float vright, float vbottom)
{
	left = vleft; top = vtop;
	right = vright; down = vbottom;
}

template<typename T>
void Rect<T>::set(const vec2f& leftTop, const vec2f& rightDown)
{
	left = leftTop.x; top = leftTop.y;
	right = rightDown.x; down = rightDown.y;
}

template<typename T>
vec2<T> Rect<T>::getSize() const
{
	return vec2<T>(getSizeX(), getSizeY()); 
}

template<typename T>
T Rect<T>::getSizeX() const
{ 
	return abs(right - left);
}

template<typename T>
T Rect<T>::getSizeY() const
{
	return abs(down - top);
}

template<typename T>
vec2<T> Rect<T>::getltCorner() const
{
	return vec2<T>(left, top); 
}

template<typename T>
vec2<T> Rect<T>::getrtCorner() const
{ 
	return vec2<T>(right, top); 
}

template<typename T>
vec2<T> Rect<T>::getldCorner() const
{ 
	return vec2<T>(left, down);
}

template<typename T>
vec2<T> Rect<T>::getrdCorner() const
{ 
	return vec2<T>(right, down);; 
}

template<typename T>
bool Rect<T>::isInside(const vec2<T>& p) const
{
	if (p.x < left || p.x > right ||
		p.y < top  || p.y > down)
	{
		return false;
	}

	return true;
}

template<typename T>
bool Rect<T>::isZero() const 
{
	return (left == 0.0f && right == 0.0f && top == 0.0f && down == 0.0f);
}

//some defines
#define fRect Rect<float>
#define iRect Rect<int>

CLOSE_O2_NAMESPACE

#endif //RECT_H