#ifndef MATH_H
#define MATH_H

#include "util/public_namespace.h"
#include <float.h>
#include <math.h>

OPEN_O2_NAMESPACE
	
#undef min
#undef max
#undef clamp

enum InterpolationType { IT_LINEAR = 0, IT_FORCIBLE, IT_EASY_IN, IT_EASY_OUT, IT_EASY_IN_OUT, IT_CURVE };
	
/* Basic Math functions. */
template<typename T>
T sign(const T& value)
{
	if (value < 0) 
		return (T)(-1);

	return (T)1;
}
	
template<typename T>
T clamp(const T& value, const T& minv, const T& maxv)
{
	if (value < minv)
		return minv;
	else if (value > maxv)
		return maxv;

	return value;
}

template<typename T>
void mswap(T& v1, T& v2)
{
	T x = v1;
	v1 = v2;
	v2 = x;
}

template<typename T>
T min(const T& v1, const T& v2)
{
	if (v1 < v2)
		return v1;
	return v2;
}

template<typename T>
T max(const T& v1, const T& v2)
{
	if (v1 > v2)
		return v1;
	return v2;
}

template<typename T>
void tswap(T& v1, T& v2)
{
	T tmp = v1;
	v1 = v2;
	v2 = tmp;
}

template<typename T>
T abs(const T& value)
{
	if (value < 0) 
		return -value;

	return value;
}

template<typename T>
T random(const T& minValue = 0, const T& maxValue = 1)
{
	return (T)((float)rand()/RAND_MAX*(float)(maxValue - minValue) + (float)minValue);
}

template<typename T>
T interpolate(const T& a, const T& b, float coef)
{
	return (b - a)*coef + a;
}

template<typename T>
T interpolateBezier(const T& a, const T& b, const T& c, const T&d, float coef)
{
	float m = 1 - coef;
	float n = m*m;
	float o = n*m;
	float p = coef*coef;
	float r = p*coef;

	return a*o + b*3.0f*coef*n + c*3.0f*p*m + d*r;
}

inline bool equals(float a, float b)
{
	float x = a - b;
	return x*x < FLT_EPSILON*FLT_EPSILON;
}

template<typename T>
inline bool equals(const T& a, const T& b)
{
	return a == b;
}

/* Trigonometry. */
#define PI 3.1415926535897932384626433832795f

inline float deg2rad(const float& value)
{
	return value*( PI/180.0f );
}

inline float rad2deg(const float& value)
{
	return value*( 180.0f/PI );
}

inline float f_sin(float r) 
{
	return sinf(r);
}

/* Matricies. */
void orthoProjMatrix(float* mat, float left, float right, float bottom, float top, float nearz, float farz);

CLOSE_O2_NAMESPACE

#endif //MATH_H