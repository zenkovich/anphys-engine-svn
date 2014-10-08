#ifndef VECTOR2_H
#define VECTOR2_H

#include "util/public_namespace.h"

OPEN_O2_NAMESPACE

#include <math.h>

template<typename T>
struct vec2
{
	T x, y;

public:
	vec2();
	vec2(T vx, T vy);

	vec2 operator+(const vec2& v) const;
	vec2 operator+=(const vec2& v);
	vec2 operator-(const vec2& v) const;
	vec2 operator-=(const vec2& v);
	vec2 operator*(T v) const;
	vec2 operator*=(T v);
	vec2 operator/(float v) const;
	vec2 operator/(const vec2& v) const;
	vec2 operator/=(T v);
	float operator*(const vec2& v) const;
	float operator*=(const vec2& v) const;
	bool operator==(const vec2& v) const;
	bool operator!=(const vec2& v) const;
	vec2 operator^(T s) const;
	float operator^(const vec2& v) const;

	T& operator[](int idx);

	vec2 scale(const vec2& v) const;
	vec2 invScale(const vec2& v) const;

	T& get(int i);
	void set(T vx, T vy);

	T dot(const vec2& v) const;
	T length() const;

	vec2 normalize() const;
	vec2 Rotate(float rad) const;
	vec2 Rotate(float cs, float sn) const;
	void Rotate(float rad, const vec2& c);

	vec2 Inv(bool bx = true, bool by = true)  const;
	vec2 InvX() const;
	vec2 InvY() const;

	template<typename T2>
	vec2<T2> castTo() const
	{
		return vec2<T2>((T2)x, (T2)y);
	}

	template<typename RT>
	operator vec2<RT>() 
	{
		return vec2<RT>((RT)x, (RT)y);
	}
};

template<typename T>
vec2<T> operator^(float s, const vec2<T>& a)
{
	return vec2<T>(-s*a.y, s*a.x);
}

template<typename T>
float length(const vec2<T>& a, const vec2<T>& b)
{ 
	return (b - a).length();
}

template<typename T>
vec2<T> proj(const vec2<T>& a, const vec2<T>& b)
{
	vec2<T> n = b.normalize();

	return n*(n*a);
}

template<typename T>
vec2<T> scale(const vec2<T>& a, const vec2<T>& b)
{
	return vec2(a.x*b.x, a.y*b.y);
}

template<typename T>
vec2<T> rotateVec(float rad, const vec2<T>& v)
{ 
	v.Rotate(rad); return v; 
}

//implementation
template<typename T>
vec2<T>::vec2()
{
	x = y = 0; 
}

template<typename T>
vec2<T>::vec2(T vx, T vy)
{
	x = vx; y = vy;
}

template<typename T>
vec2<T> vec2<T>::operator+(const vec2<T>& v) const
{ 
	return vec2(x + v.x, y + v.y);
}

template<typename T>
vec2<T> vec2<T>::operator+=(const vec2<T>& v) 
{
	*this = *this + v; 
	return *this;
}

template<typename T>
vec2<T> vec2<T>::operator-(const vec2<T>& v) const
{
	return vec2(x - v.x, y - v.y);
}

template<typename T>
vec2<T> vec2<T>::operator-=(const vec2<T>& v) 
{ 
	*this = *this - v; return *this; 
}

template<typename T>
vec2<T> vec2<T>::operator*(T v) const
{ 
	return vec2(x*v, y*v); 
}

template<typename T>
vec2<T> vec2<T>::operator*=(T v)
{ 
	*this = (*this)*v; 
	return *this;
}

template<typename T>
vec2<T> vec2<T>::operator/(float v) const
{
	return *this*(1.0f/v);
}

template<typename T>
vec2<T> vec2<T>::operator/(const vec2<T>& v) const
{
	return vec2(x/v.x, y/v.y);
}

template<typename T>
vec2<T> vec2<T>::operator/=(T v) 
{ 
	*this = *this/v; 
	return *this; 
}

template<typename T>
float vec2<T>::operator*(const vec2<T>& v) const
{ 
	return (x*v.x + y*v.y);
}

template<typename T>
float vec2<T>::operator*=(const vec2<T>& v) const
{
	return *this*v; 
}

template<typename T>
bool vec2<T>::operator==(const vec2<T>& v) const
{ 
	if (abs(x - v.x) > (T)0.0001f || abs(y - v.y) > (T)0.0001f) return false; 		
	return true; 
}

template<typename T>
bool vec2<T>::operator!=(const vec2<T>& v) const
{ 
	return !(v == *this);
}

template<typename T>
vec2<T> vec2<T>::operator^(T s) const
{
	return vec2(s*y, -s*x); 
}

template<typename T>
float vec2<T>::operator^(const vec2<T>& v) const
{
	return v.x*y - v.y*x; 
}

template<typename T>
T& vec2<T>::operator[](int idx) 
{ 
	if (idx == 0)
		return x; 
	return y; 
}

template<typename T>
vec2<T> vec2<T>::scale(const vec2<T>& v) const
{
	return vec2(x*v.x, y*v.y);
}	

template<typename T>
vec2<T> vec2<T>::invScale(const vec2<T>& v) const
{
	return vec2(x/v.x, y/v.y);
}

template<typename T>
T& vec2<T>::get(int i) 
{ 
	if (i == 0) return x; 
	if (i == 1) return y;
	return x; 
}

template<typename T>
void vec2<T>::set(T vx, T vy)
{
	x = vx; y = vy;
}

template<typename T>
T vec2<T>::dot(const vec2<T>& v) const
{
	return *this*v;
}

template<typename T>
T vec2<T>::length() const
{
	return sqrt((*this)*(*this));
}

template<typename T>
vec2<T> vec2<T>::normalize() const
{ 
	T ln = length();
	if (ln > 0) 
		return *this/ln; 
	else 
		return vec2(0, 0); 
}

template<typename T>
vec2<T> vec2<T>::Rotate(float rad) const
{
	float cs = cosf(rad), 
			sn = sinf(rad);

	vec2 v( (T)(cs*x - sn*y), (T)(sn*x + cs*y) );

	return v;
}

template<typename T>
vec2<T> vec2<T>::Rotate(float cs, float sn) const
{
	vec2 v( (T)(cs*x - sn*y), (T)(sn*x + cs*y) );
	v = v^(T)(1.0f);

	return v;
}

template<typename T>
void vec2<T>::Rotate(float rad, const vec2<T>& c) 
{
	*this -= c;
	Rotate(rad); 
	*this += c;
}	

template<typename T>
vec2<T> vec2<T>::Inv(bool bx = true, bool by = true)  const
{
	vec2 r = *this; 
	if (bx) r.x = -r.x; 
	if (by) r.y = -r.y;
	return r; 
} 

template<typename T>
vec2<T> vec2<T>::InvX() const
{ 
	vec2 r = *this;
	r.x = -r.x; 
	return r; 
}

template<typename T>
vec2<T> vec2<T>::InvY() const
{ 
	vec2 r = *this;
	r.y = -r.y;
	return r; 
}

//some definiotions
#define vec2f vec2<float>
#define vec2i vec2<int>

CLOSE_O2_NAMESPACE

#endif //VECTOR2_H