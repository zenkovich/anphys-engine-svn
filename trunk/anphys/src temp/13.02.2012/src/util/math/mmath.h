#ifndef MATH_H
#define MATH_H

#include "../../engine/engine_options.h"
#include <stdlib.h>

#include "vector.h"
#include "matrix.h"
#include "quat.h"
#include "ray.h"
#include "addMath.h"
#include "vec2.h"
#include "vertex.h"
#include "color.h"

#ifdef PLATFORM_WIN32
#include <windows.h>
#endif


#define Pi 3.14159f
#define Pi180 0.01745329251994f
#define MDif 0.00000001f
#define NULL 0


inline float random(float min, float max)
{
	return (rand()%1000)*0.001f*(max - min) + min; //woomen brain :)
}

inline float sqr(float f) { return f*f; }

inline float Sqroot(float x) 
{
	int expo, i;
	float a, b;

	if (x <= 0) return 0;

	x = frexp(x, &expo);
	if (expo&1) { x*=2.0f; expo--; }

	a=1.0f;

	for (i = 4; i > 0; i--) 
	{
		b = x/a; a += b; a *= 0.5f;
	}

	a = ldexp(a, expo/2);
	return a;
}

struct poly3
{
	int a, b, c;
	inline poly3() { a = b = c = 0; }
	inline poly3(int A, int B, int C) { a = A; b = B; c = C; }
};

struct poly4
{
	int a, b, c, d;
	inline poly4() { a = b = c = d = 0; }
	inline poly4(int A, int B, int C, int D) { a = A; b = B; c = C; d = D; }
};

struct polygon
{
	int a, b, c;
	vec3 n;

	inline polygon() { a = b = c = 0; }
	inline polygon(int A, int B, int C) { a = A; b = B; c = C; }
	inline polygon(int A, int B, int C, vec3 norm) { a = A; b = B; c = C; n = norm; }
};

inline void copyPolygons(int number, poly3 *in, polygon *out)
{
	for (int i = 0; i < number; i++)
		out[i] = polygon(in[i].a, in[i].b, in[i].c, vec3(0,1,0));
}

inline void copyPolygons(poly3 *out, polygon *in, int number)
{
	for (int i = 0; i < number; i++)
		out[i] = poly3(in[i].a, in[i].b, in[i].c);
}

inline mat3x3 nullMatr()
{
	return mat3x3(1,0,0,  0,1,0,  0,0,1);
}

inline float sign(float v) { if(v>0) return 1.0f; else return -1.0f; }
inline float fmin(float a, float b) { if(a<b) return a; return b; }
inline float fmax(float a, float b) { if(a>b) return a; return b; }

inline int imin(int a, int b) { if(a<b) return a; return b; }
inline int imax(int a, int b) { if(a>b) return a; return b; }
inline void swap(int &a, int &b) { int c=a; a=b; b=c; }

inline float fclamp(float a, float b, float c) 
{
	return fmax(b, fmin(a, c));
}

inline mat3x3 RotatedMatrix(float rx, float ry, float rz)
{
	mat3x3 rt;
	rt.SetRotationXYZ(rx, ry, rz);
	return rt;
}

inline mat3x3 ScaledMatrix(float sx, float sy, float sz)
{
	mat3x3 rt;
	rt.SetScale(sx,sy,sz);
	return rt;
}

inline mat3x3 m4to3(mat4x4 m)
{
	return mat3x3(m[0][0], m[0][1], m[0][2],
		          m[1][0], m[1][1], m[1][2],
				  m[2][0], m[2][1], m[2][2]);
}

inline mat4x4 m3to4(mat3x3 m)
{
	return mat4x4(m[0][0], m[0][1], m[0][2], 0,
		          m[1][0], m[1][1], m[1][2], 0,
				  m[2][0], m[2][1], m[2][2], 0,
				  0,       0,       0,       1);
}

inline mat4x4 m3to4(mat3x3 m, vec3 pos)
{
	return mat4x4(m[0][0], m[0][1], m[0][2], 0,
		          m[1][0], m[1][1], m[1][2], 0,
				  m[2][0], m[2][1], m[2][2], 0,
				  pos.x,   pos.y,   pos.z,1);
}

inline vec3 getm4pos(mat4x4 m)
{
	return vec3(m[3][0], m[3][1], m[3][2]);
}

inline mat3x3 vectorOrient(vec3 vec)
{
	vec3 osX = (vec^vec3(1, 1, 1)).normalize();
	vec3 osZ = (osX^vec).normalize()*(-1.0f);
	vec3 osY = vec.normalize();

	return mat3x3(osX.x, osX.y, osX.z,
		          osY.x, osY.y, osY.z,
				  osZ.x, osZ.y, osZ.z);
}

inline mat3x3 scaledVectorOrient(vec3 vec, vec3 scale)
{
	return ScaledMatrix(scale.x, scale.y, scale.z)*vectorOrient(vec);
}

inline mat3x3 scaledVectorOrient(vec3 vec, float w = 0.1f, float h = 0.1f)
{
	return scaledVectorOrient(vec, vec3(w, vec.len(), h));
}

inline bool projPointInTriangle(vec3 pa, vec3 pb, vec3 pc, vec3 norm, vec3 point)
{
	float sign1 = sign((point - pa)*((pb - pa)^norm));
	float sign2 = sign((point - pb)*((pc - pb)^norm));
	float sign3 = sign((point - pc)*((pa - pc)^norm));

	if (sign1 != sign2 || sign2 != sign3 || sign3 != sign1) return false;

	return true;
}

inline float f3max(float a, float b, float c, int &i)
{
	float m;
	int im;
	if (a > b)
	{
		m = a; im = 0;
	}
	else
	{
		m = b; im = 1;
	}

	if (m > c)
	{
		i = im;
		return m;
	}
	else
	{
		i = 2;
		return c;
	}

	i = 0;
	return a;
}

inline float f3min(float a, float b, float c, int &i)
{
	float m;
	int im;
	if (a < b)
	{
		m = a; im = 0;
	}
	else
	{
		m = b; im = 1;
	}

	if (m < c)
	{
		i = im;
		return m;
	}
	else
	{
		i = 2;
		return c;
	}

	i = 0;
	return a;
}

inline void DistLines(vec3 a1p, vec3 b1p, vec3 a2p, vec3 b2p, float *alpha, float *bet)
{
	vec3 l1orig = a1p, l1dir = b1p;
	vec3 l2orig = a2p, l2dir = b2p;

	vec3 diff = l1orig - l2orig;
	float a01 =- (l1dir*l2dir);
	float b0 = (diff*l1dir);
	float c = (diff*diff);
	float det = (float)fabs((1.0f - a01*a01));
	float b1, s0, s1;
	
	if (det >= 0.000000001f)
	{
		b1 =- (diff*l2dir);
		float InvDet = 1.0f/det;
		s0 = (a01*b1 - b0)*InvDet;
		s1 = (a01*b0 - b1)*InvDet;
	}
	else
	{
		s0 =- b0;
		s1 = 0;
	}

	*alpha = s0;
	*bet = s1;
}

inline void DistLines(vec3 a1p, vec3 b1p, vec3 a2p, vec3 b2p, vec3 &pa, vec3 &pb,float &la, float &lb)
{
	vec3 l1orig = a1p, l1dir = b1p;
	vec3 l2orig = a2p, l2dir = b2p;

	vec3 diff = l1orig - l2orig;
	float a01 =- (l1dir*l2dir);
	float b0 = (diff*l1dir);
	float c = (diff*diff);
	float det = (float)fabs((1.0f - a01*a01));
	float b1, s0, s1;
	
	if (det >= 0.000000001f)
	{
		b1 =- (diff*l2dir);
		float InvDet = 1.0f/det;
		s0 = (a01*b1 - b0)*InvDet;
		s1 = (a01*b0 - b1)*InvDet;
	}
	else
	{
		s0 =- b0;
		s1 = 0;
	}

	pa = l1orig + l1dir*s0;
	pb = l2orig + l2dir*s1;
	la = s0; lb = s1;
}

inline float absf(float f) { if (f < 0) return -f; else return f; }

inline vec3 operator*(vec3 v, mat3x3 m) { return m.transform(v); }
inline vec3 operator*=(vec3 v, mat3x3 m) { return m.transform(v); }
inline vec3 operator*(vec3 v, mat4x4 m) { return m.transform(v); }
inline vec3 operator*=(vec3 v, mat4x4 m) { return m.transform(v); }
inline vec3 operator*(vec3 v, quat q) 
{
	quat qv(v, 0);
	quat r = q*qv*q.inverse();
	return r.v;
}

inline mat3x3 quat2matrix(quat q)
{
	float x2  = q.v.x*2, y2  = q.v.y*2,  z2  = q.v.z*2,  w2 = q.w*2;
	float xx2 =q.v.x*x2, yy2 = q.v.y*y2, zz2 = q.v.z*z2;
	float xy2 =q.v.x*y2, zw2 = q.v.z*w2, xz2 = q.v.x*z2;
	float yw2 =q.v.y*w2, yz2 = q.v.y*z2, xw2 = q.v.x*w2;

	return mat3x3(1.0f - yy2 - zz2,	xy2 - zw2,		  xz2 + yw2,
				  xy2 + zw2,		1.0f - xx2 - zz2, yz2 - xw2,
				  xz2 - yw2,		yz2 + xw2,		  1.0f - xx2 - yy2);
}

inline quat matrix2quat(mat3x3 m)
{
	quat ret;
	float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];  
	if (trace > 0)
	{
		float troot = sqrtf(trace + 1.0f);
		ret.w = 0.5f*troot;
		troot = 0.5f/troot; 
		ret.v.x = (m.m[2][1] - m.m[1][2])*troot;
		ret.v.y = (m.m[0][2] - m.m[2][0])*troot;
		ret.v.z = (m.m[1][0] - m.m[0][1])*troot;
	}
	else
	{
		static int next[3] = {1, 2, 0};
		int i = 0;
		if (m.m[1][1] > m.m[0][0]) i = 1;
		if (m.m[2][2] > m[i][i])   i = 2;
  
		int j = next[i];
		int k = next[j];

		float troot = sqrtf(m[i][i] - m[j][j] - m[k][k] + 1.0f);
		if (i == 0) ret.v.x = 0.5f*troot;
		if (i == 1) ret.v.y = 0.5f*troot;
		if (i == 2) ret.v.z = 0.5f*troot;

		troot = 0.5f/troot;

		ret.w = (m[k][j] - m[j][k])*troot;

		if (j == 0) ret.v.x = (m[i][j] + m[j][i])*troot;
		if (j == 1) ret.v.y = (m[i][j] + m[j][i])*troot;
		if (j == 2) ret.v.z = (m[i][j] + m[j][i])*troot;
		if (k == 0) ret.v.x = (m[i][k] + m[k][i])*troot;
		if (k == 1) ret.v.y = (m[i][k] + m[k][i])*troot;
		if (k == 2) ret.v.z = (m[i][k] + m[k][i])*troot;
	}

	return ret;
}

inline vec3 projectPoint2Plane(vec3 p, vec3 a, vec3 b, vec3 c, vec3 n)
{
	vec3 rr;
	IntersectLinePolygon(a, b, c, p, p - n*200.0f, &rr, ((b - a)^(c - a)).normalize());
	return rr;
}

/*inline vec3 vec2(float x, float y)
{
	return vec3(x,y);
}*/

struct fRect
{
	vec2 leftTop, rightDown;

	inline fRect() { leftTop = rightDown = vec2(0); }
	inline fRect(vec2 lt, vec2 rd) { leftTop = lt; rightDown = rd; }
	inline fRect(float left, float top, float right, float bottom)
	{
		leftTop = vec2(left, top);
		rightDown = vec2(right, bottom);
	}
#ifdef PLATFORM_WIN32
	inline fRect(RECT rt)
	{
		leftTop = vec2((float)rt.left, (float)rt.top);
		rightDown = vec2((float)rt.right, (float)rt.bottom);
	}

	inline void operator=(RECT rt)
	{
		leftTop = vec2((float)rt.left, (float)rt.top);
		rightDown = vec2((float)rt.right, (float)rt.bottom);
	}

	inline RECT getWinRect() 
	{
		RECT rt = { (int)leftTop.x, (int)leftTop.y, (int)rightDown.x, (int)rightDown.y };
		return rt;
	}
#endif
	inline void operator=(fRect rt)
	{
		leftTop = rt.leftTop;
		rightDown = rt.rightDown;
	}

	inline vec2 getSize() { return vec2(getSizeX(), getSizeY()); }
	inline float getSizeX() { return absf(rightDown.x - leftTop.x); }
	inline float getSizeY() { return absf(rightDown.y - leftTop.y); }
	inline vec2 getltCorner() { return leftTop; }
	inline vec2 getrtCorner() { return vec2(rightDown.x, leftTop.y); }
	inline vec2 getldCorner() { return vec2(leftTop.x, rightDown.y); }
	inline vec2 getrdCorner() { return rightDown; }

	inline bool inRect(vec2 p)
	{
		if (p.x < leftTop.x || p.x > rightDown.x ||
		    p.y < leftTop.y || p.y > rightDown.y) return false;
		return true;
	}

	inline fRect plusVector(vec2 v)
	{
		return fRect(leftTop + v, rightDown + v);
	}
};



#endif

