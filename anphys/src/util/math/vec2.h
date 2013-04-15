#ifndef VEC2_H
#define VEC2_H


#include <math.h>


#define Pi 3.14159f
#define MDif 0.00000001f
#define NULL 0

inline float fssqrt2(float x)
{
  _asm{
    MOV EAX, x
    SUB    EAX, 0C0800000H        
    TEST  EAX, 000800000H        
    MOV   ECX, EAX 
    JZ     Z0 
    NEG    EAX 
  Z0:      AND      EAX, 000FFFFFFH        
    SHR    ECX, 1 
    MUL    EAX 
    NEG    EDX 
    LEA    EAX, [ECX+EDX*8] 
    LEA    EDX, [EDX+EDX*8] 
    LEA    EAX, [EAX+EDX*4]
    MOV    x, EAX}
	return x;
}

struct vec2
{
	float x, y;

	inline vec2()
	{
		x = y = 0; 
	}

	inline vec2(float vx, float vy)
	{
		x = vx; y = vy;
	}

	inline vec2 operator+(const vec2& v) const
	{ 
		return vec2(x + v.x, y + v.y);
	}

	inline vec2 operator+=(const vec2& v) 
	{
		*this = *this + v; 
		return *this;
	}

	inline vec2 operator-(const vec2& v) const
	{
		return vec2(x - v.x, y - v.y);
	}

	inline vec2 operator-=(const vec2& v) 
	{ 
		*this = *this - v; return *this; 
	}

	inline vec2 operator*(float v) const
	{ 
		return vec2(x*v, y*v); 
	}

	inline vec2 operator*=(float v)
	{ 
		*this = (*this)*v; 
		return *this;
	}

	inline vec2 operator/(float v) const
	{
		return *this*(1.0f/v);
	}

	inline vec2 operator/=(float v) 
	{ 
		*this = *this/v; 
		return *this; 
	}

	inline float operator*(const vec2& v) const
	{ 
		return (x*v.x + y*v.y);
	}

	inline float operator*=(const vec2& v) const
	{
		return *this*v; 
	}

	inline bool operator==(const vec2& v) const
	{ 
		if (x != v.x || y != v.y) return false; 		
		return true; 
	}

	inline bool operator!=(const vec2& v) 
	{ 
		if (x != v.x || y != v.y) return true; 
		return false;
	}

	inline vec2 operator^(float s) const
	{
		return vec2(s*y, -s*x); 
	}

	inline float operator^(const vec2& v) const
	{
		return v.x*y - v.y*x; 
	}

	inline float& operator[](int idx) 
	{ 
		if (idx == 0) return x; return y; 
	}

	inline vec2 scale(const vec2& v) const
	{
		return vec2(x*v.x, y*v.y);
	}	

	inline float& get(int i) 
	{ 
		if (i == 0) return x; 
		if (i == 1) return y;
		return x; 
	}

	inline void set(int i, float n) 
	{ 
		if (i == 0) x = n; 
		if (i == 1) y = n; 
	}

	inline float dot(const vec2& v) const
	{
		return *this*v;
	}

	inline float len() const
	{
		return fssqrt2((*this)*(*this));
	}

	inline vec2 normalize() const
	{ 
		float ln = len();
		if (ln > MDif) return *this/ln; 
		else return vec2(0, 0); 
	}

	inline vec2 Rotate(float rad) const
	{
		float cs = cosf(rad), 
			  sn = sinf(rad);

		vec2 v(cs*x - sn*y, sn*x + cs*y);

		return v;
	}

	inline vec2 Rotate(float cs, float sn) const
	{
		vec2 v(cs*x - sn*y, sn*x + cs*y);
		v = v^1.0f;

		return v;
	}

	inline void Rotate(float rad, const vec2& c) 
	{
		*this -= c;
		Rotate(rad); 
		*this += c;
	}	

	inline vec2 Inv(bool bx, bool by, bool bz)  const
	{
		vec2 r = *this; 
		if (bx) r.x = -r.x; 
		if (by) r.y = -r.y;
		return r; 
	} 

	inline vec2 InvX() const
	{ 
		vec2 r = *this;
		r.x = -r.x; 
		return r; 
	}

	inline vec2 InvY() const
	{ 
		vec2 r = *this;
		r.y = -r.y;
		return r; 
	}
};

inline vec2 operator^(float s, const vec2& a)
{
	return vec2(-s*a.y, s*a.x);
}

inline float len(const vec2& a, const vec2& b)
{ 
	return (b - a).len();
}

inline vec2 proj(const vec2& a, const vec2& b)
{
	vec2 n = b.normalize();

	return n*(n*a);
}

inline vec2 scale(const vec2& a, const vec2& b)
{
	return vec2(a.x*b.x, a.y*b.y);
}

inline vec2 rotateVec(float rad, const vec2& v)
{ 
	v.Rotate(rad); return v; 
}

inline float rad(float deg) { return deg*0.01745329251994329576f; }
inline float deg(float rad) { return rad*57.2957795130823208767f; }

#endif