#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>


#define MDif 0.00000001f

inline float fssqrt(float x)
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

struct vec3
{
	float x,y,z;

	inline vec3() { x = y = z = 0; }
	inline vec3(float v) { x = y = z = v; }
	inline vec3(float vx, float vy, float vz) { x = vx; y = vy; z = vz; }
	inline vec3(float vx, float vy) { x  =vx; y = vy; z = 1; }
	//inline vec3(float v[3]) { x=v[0];y=v[1];z=v[2]; }
	//inline vec3(vec3 v) { x=v.x;y=v.y;z=v.z; }

	inline vec3 operator+(const vec3& v) const 
	{ 
		return vec3(x + v.x, y + v.y, z + v.z); 
	}

	inline vec3 operator+=(const vec3& v) 
	{ 
		*this = *this + v; return *this; 
	}

	inline vec3 operator-(const vec3& v) const 
	{ 
		return vec3(x - v.x, y - v.y, z - v.z); 
	}

	inline vec3 operator-=(const vec3& v)
	{ 
		*this = *this - v; return *this; 
	}

	inline vec3 operator*(const float v) const 
	{
		return vec3(x*v, y*v, z*v); 
	}

	inline vec3 operator*=(const float v) 
	{
		*this = *this*v; return *this;
	}

	inline vec3 operator/(const float v) const 
	{ 
		return *this*(1.0f/v); 
	}

	inline vec3 operator/=(const float v) 
	{
		*this = *this/v; return *this;
	}

	inline float operator*(const vec3& v) const 
	{ 
		return (x*v.x + y*v.y + z*v.z);
	}

	inline float operator*=(const vec3& v) const 
	{ 
		return *this*v; 
	}

	inline vec3 operator^(const vec3& v) const 	
	{ 
		return vec3(v.y*z - y*v.z, v.z*x - z*v.x, v.x*y - x*v.y); 
	}
	
	inline bool operator==(const vec3& v) const 
	{ 
		if (x != v.x || y != v.y || z != v.z) return false; 
		return true; 
	}

	inline bool operator!=(const vec3& v) const
	{ 
		if (x != v.x || y != v.y || z != v.z) return true; 
		return false; 
	}

	inline float& operator[](const int idx)
	{
		if (idx == 0) return x; if (idx == 1) return y; return z; 
	}

	inline vec3 scale(const vec3& v) const 
	{ 
		return vec3(x*v.x, y*v.y, z*v.z);
	}

	inline float get(int i) const
	{ 
		if (i == 0) return x; if (i == 1) return y; return z;  
	}

	inline void set(int i, float n)
	{ 
		if (i == 0) x = n;
		else if (i == 1) y = n; 
		else if (i == 2) z = n; 
	}

	inline float dot(const vec3& v) const { return *this*v; }
	inline vec3  cross(const vec3& v) const {return *this^v; }

	inline float len() const { return fssqrt((*this)*(*this)); }
	inline vec3 normalize() const { float ln = len(); if (ln > MDif) return *this/ln; else return vec3(0); }

	inline void RotateX(float rad)
	{
		float cs = cosf(rad), 
			  sn  =sinf(rad);
		vec3 v(x, cs*y - sn*z, sn*y + cs*z);

		*this = v;
	}

	inline void RotateY(float rad)
	{
		float cs = cosf(rad), 
			  sn = sinf(rad);
		vec3 v(cs*x - sn*z, y, sn*x + cs*z);

		*this = v;
	}

	inline void RotateZ(float rad)
	{
		float cs = cosf(rad), 
			  sn = sinf(rad);
		vec3 v(cs*x - sn*y, sn*x + cs*y, z);

		*this = v;
	}

	inline void RotateXYZ(float rx, float ry, float rz)
	{
		RotateX(rx); RotateY(rx); RotateZ(rx);
	}

	inline void RotateX(float rad, vec3 c) { *this -= c; RotateX(rad); *this += c; }
	inline void RotateY(float rad, vec3 c) { *this -= c; RotateY(rad); *this += c; }
	inline void RotateZ(float rad, vec3 c) { *this -= c; RotateZ(rad); *this += c; }	

	inline void RotateXYZ(float rx, float ry, float rz, vec3 c) { *this -= c; RotateXYZ(rx, ry, rz); *this += c; }

	inline vec3 Inv(bool bx, bool by, bool bz) const
	{ 
		vec3 r = *this; if (bx) r.x = -r.x; if (by) r.y = -r.y; if (bz) r.z = -r.z; return r; 
	} 

	inline vec3 InvX() const { vec3 r = *this; r.x = -r.x; return r; }
	inline vec3 InvY() const { vec3 r = *this; r.y = -r.y; return r; }
	inline vec3 InvZ() const { vec3 r = *this; r.z = -r.z; return r; }
};

inline float len(const vec3& a, const vec3& b) { return (b - a).len(); }

inline vec3 proj(const vec3& a, const vec3& b)
{
	vec3 n = b.normalize();
	return n*(n*a);
}

inline vec3 projectPoint2Plane(const vec3& p, const vec3&& pn, const vec3 pp)
{
	float prj = (p - pp)*pn;
	vec3 r = (p - (pn*prj));

	return r;
}


inline vec3 scale(const vec3& a, const vec3& b)
{
	return vec3(a.x*b.x, a.y*b.y, a.z*b.z);
}
inline vec3 rotateVecX(float rad, const vec3& v) { vec3 res = v; res.RotateX(rad); return res; }
inline vec3 rotateVecY(float rad, const vec3& v) { vec3 res = v; res.RotateY(rad); return res; }
inline vec3 rotateVecZ(float rad, const vec3& v) { vec3 res = v; res.RotateZ(rad); return res; }

#endif
