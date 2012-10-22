#ifndef VEC2_H
#define VEC2_H


#include <math.h>


#define Pi 3.14159f
#define Pi180 0.01745329251994f
#define MDif 0.00000001f
#define NULL 0


struct vec2
{
	float x, y;

	inline vec2() { x = y = 0; }
	inline vec2(float v) { x = y = v; }
	inline vec2(float vx, float vy) { x = vx; y = vy;}

	inline vec2 operator+(vec2 v) { return vec2(x + v.x, y + v.y); }
	inline vec2 operator+=(vec2 v) { *this = *this + v; return *this; }
	inline vec2 operator-(vec2 v) { return vec2(x - v.x, y - v.y); }
	inline vec2 operator-=(vec2 v) { *this = *this - v; return *this; }
	inline vec2 operator*(float v) { return vec2(x*v, y*v); }
	inline vec2 operator*=(float v) { *this = *this*v; return *this; }
	inline vec2 operator/(float v) { return *this*(1.0f/v); }
	inline vec2 operator/=(float v) { *this = *this/v; return *this; }
	inline float operator*(vec2 v) { return (x*v.x + y*v.y); }
	inline float operator*=(vec2 v) { return *this*v; }
	inline bool operator==(vec2 v) { if (x != v.x || y != v.y) return false; else return true; }
	inline bool operator!=(vec2 v) { if (x != v.x || y != v.y) return true; else return false; }
	inline vec2 operator^(float s) { return vec2(s*y, -s*x); }
	inline float operator^(vec2 v) { return v.x*y - v.y*x; }

	inline vec2 umnoj(vec2 v) { return vec2(x*v.x, y*v.y); }
	

	inline float get(int i) { if (i == 0) return x; if (i == 1) return y; return x; }
	inline void  set(int i, float n) { if (i == 0) x = n; if (i == 1) y=n; }

	inline float dot(vec2 v) { return *this*v; }

	inline float len() { return fssqrt((*this)*(*this)); }
	inline vec2 normalize() { float ln = len(); if (ln > MDif) return *this/ln; else return vec2(0); }

	inline vec2 Rotate(float rad)
	{
		float cs = cosf(rad), 
			  sn = sinf(rad);

		vec2 v(cs*x - sn*y, sn*x + cs*y);

		return v;
	}

	inline vec2 Rotate(float cs, float sn)
	{
		vec2 v(cs*x - sn*y, sn*x + cs*y);
		v = v^1.0f;

		return v;
	}

	inline void Rotate(float rad, vec2 c) { *this -= c; Rotate(rad); *this += c; }	

	inline vec2 Inv(bool bx, bool by, bool bz) { vec2 r = *this; if (bx) r.x = -r.x; if (by) r.y = -r.y;; return r; } 
	inline vec2 InvX() { vec2 r = *this; r.x = -r.x; return r; }
	inline vec2 InvY() { vec2 r = *this; r.y = -r.y; return r; }
};

inline vec2 operator^(float s, const vec2 a)
{
	return vec2(-s*a.y, s*a.x);
}

inline float len(vec2 a, vec2 b) { return (b - a).len(); }

inline vec2 proj(vec2 a, vec2 b)
{
	vec2 n = b.normalize();

	return n*(n*a);
}

inline vec2 scale(vec2 a, vec2 b)
{
	return vec2(a.x*b.x, a.y*b.y);
}

inline vec2 rotateVec(float rad, vec2 v) { v.Rotate(rad); return v; }

inline float rad(float deg) { return deg*Pi180; }
inline float deg(float rad) { return rad/Pi180; }

#endif