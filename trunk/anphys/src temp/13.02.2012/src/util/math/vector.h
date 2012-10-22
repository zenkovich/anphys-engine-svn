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

	inline virtual vec3 operator+(vec3 v) { return vec3(x + v.x, y + v.y, z + v.z); }
	inline virtual vec3 operator+=(vec3 v) { *this = *this + v; return *this; }
	inline virtual vec3 operator-(vec3 v) { return vec3(x - v.x, y - v.y, z - v.z); }
	inline virtual vec3 operator-=(vec3 v) { *this = *this - v; return *this; }
	inline virtual vec3 operator*(float v) { return vec3(x*v, y*v, z*v); }
	inline virtual vec3 operator*=(float v) { *this = *this*v; return *this; }
	inline virtual vec3 operator/(float v) { return *this*(1.0f/v); }
	inline virtual vec3 operator/=(float v) { *this = *this/v; return *this; }
	inline virtual float operator*(vec3 v) { return (x*v.x + y*v.y + z*v.z); }
	inline virtual float operator*=(vec3 v) { return *this*v; }
	inline virtual vec3 operator^(vec3 v) { return vec3(v.y*z - y*v.z, v.z*x - z*v.x, v.x*y - x*v.y); }
	inline virtual bool operator==(vec3 v) { if (x != v.x || y != v.y || z != v.z) return false; else return true; }
	inline virtual bool operator!=(vec3 v) { if (x != v.x || y != v.y || z != v.z) return true; else return false; }

	inline virtual vec3 umnoj(vec3 v) { return vec3(x*v.x, y*v.y, z*v.z); }

	//inline virtual bool near(vec3 v, float maxDiff) { if (fabs(x - v.x) > maxDiff || fabs(y - v.y) > maxDiff || fabs(z - v.z) > maxDiff) return false; return true; }
	

	inline virtual float get(int i) { if (i == 0) return x; if (i == 1) return y; if (i == 2) return z; return x; }
	inline virtual void  set(int i, float n) { if (i == 0) x = n; if (i == 1) y = n; if (i == 2) z = n; }

	inline virtual float dot(vec3 v) { return *this*v; }
	inline virtual vec3  cross(vec3 v) {return *this^v; }

	inline virtual float len() { return fssqrt((*this)*(*this)); }
	inline virtual vec3 normalize() { float ln = len(); if (ln > MDif) return *this/ln; else return vec3(0); }

	inline virtual void RotateX(float rad)
	{
		float cs = cosf(rad), 
			  sn  =sinf(rad);
		vec3 v(x, cs*y - sn*z, sn*y + cs*z);

		*this = v;
	}

	inline virtual void RotateY(float rad)
	{
		float cs = cosf(rad), 
			  sn = sinf(rad);
		vec3 v(cs*x - sn*z, y, sn*x + cs*z);

		*this = v;
	}

	inline virtual void RotateZ(float rad)
	{
		float cs = cosf(rad), 
			  sn = sinf(rad);
		vec3 v(cs*x - sn*y, sn*x + cs*y, z);

		*this = v;
	}

	inline virtual void RotateXYZ(float rx, float ry, float rz)
	{
		RotateX(rx); RotateY(rx); RotateZ(rx);
	}

	inline virtual void RotateX(float rad, vec3 c) { *this -= c; RotateX(rad); *this += c; }
	inline virtual void RotateY(float rad, vec3 c) { *this -= c; RotateY(rad); *this += c; }
	inline virtual void RotateZ(float rad, vec3 c) { *this -= c; RotateZ(rad); *this += c; }	

	inline virtual void RotateXYZ(float rx, float ry, float rz, vec3 c) { *this -= c; RotateXYZ(rx, ry, rz); *this += c; }

	inline virtual vec3 Inv(bool bx, bool by, bool bz) { vec3 r = *this; if (bx) r.x = -r.x; if (by) r.y = -r.y; if (bz) r.z = -r.z; return r; } 

	inline virtual vec3 InvX() { vec3 r = *this; r.x = -r.x; return r; }
	inline virtual vec3 InvY() { vec3 r = *this; r.y = -r.y; return r; }
	inline virtual vec3 InvZ() { vec3 r = *this; r.z = -r.z; return r; }
};

inline float len(vec3 a, vec3 b) { return (b - a).len(); }

inline vec3 project(vec3 a, vec3 b) 
{ 
	float aa = a*a; 
	if ((float)fabs((double)aa) < MDif) aa = MDif; 

	return (a^(a*b)/aa); 
}

inline vec3 proj(vec3 a, vec3 b)
{
	vec3 n = b.normalize();
	return n*(n*a);
}

inline vec3 projectPoint2Plane(vec3 p, vec3 pn, vec3 pp)
{
	float prj = (p - pp)*pn;
	vec3 r = (p - (pn*prj));

	return r;
}


inline vec3 scale(vec3 a, vec3 b)
{
	return vec3(a.x*b.x, a.y*b.y, a.z*b.z);
}
inline vec3 rotateVecX(float rad, vec3 v) { v.RotateX(rad); return v; }
inline vec3 rotateVecY(float rad, vec3 v) { v.RotateY(rad); return v; }
inline vec3 rotateVecZ(float rad, vec3 v) { v.RotateZ(rad); return v; }
inline vec3 v(float f[3]) { return vec3(f[0],f[1],f[2]); }
inline void v(vec3 v, float *f) { f[0]=v.x; f[1]=v.y; f[2]=v.z; }
//inline float[3] v(vec3 v) { float f[3]; v(v, f); return f; }

struct fmas3
{
	float m[3];

	inline fmas3() { m[0]=0;m[1]=0;m[2]=0; }
	inline fmas3(vec3 v) { m[0]=v.x;m[1]=v.y;m[2]=v.z; }
};

#endif
