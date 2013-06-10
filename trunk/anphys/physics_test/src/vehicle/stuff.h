#ifndef STUFF_H
#define STUFF_H

#include <math.h>
#include <float.h>

namespace physics
{

struct vec3
{
	float x,y,z;

	inline vec3() { x = y = z = 0; }
	inline vec3(float v) { x = y = z = v; }
	inline vec3(float vx, float vy, float vz) { x = vx; y = vy; z = vz; }
	inline vec3(float vx, float vy) { x  =vx; y = vy; z = 1; }
	//inline vec3(float v[3]) { x=v[0];y=v[1];z=v[2]; }
	//inline vec3(vec3 v) { x=v.x;y=v.y;z=v.z; }

	inline virtual vec3 operator+(const vec3& v) const 
	{ 
		return vec3(x + v.x, y + v.y, z + v.z); 
	}

	inline virtual vec3 operator+=(const vec3& v) 
	{ 
		*this = *this + v; return *this; 
	}

	inline virtual vec3 operator-(const vec3& v) const 
	{ 
		return vec3(x - v.x, y - v.y, z - v.z); 
	}

	inline virtual vec3 operator-=(const vec3& v)
	{ 
		*this = *this - v; return *this; 
	}

	inline virtual vec3 operator*(const float v) const 
	{
		return vec3(x*v, y*v, z*v); 
	}

	inline virtual vec3 operator*=(const float v) 
	{
		*this = *this*v; return *this;
	}

	inline virtual vec3 operator/(const float v) const 
	{ 
		return *this*(1.0f/v); 
	}

	inline virtual vec3 operator/=(const float v) 
	{
		*this = *this/v; return *this;
	}

	inline virtual float operator*(const vec3& v) const 
	{ 
		return (x*v.x + y*v.y + z*v.z);
	}

	inline virtual float operator*=(const vec3& v) const 
	{ 
		return *this*v; 
	}

	inline virtual vec3 operator^(const vec3& v) const 	
	{ 
		return vec3(v.y*z - y*v.z, v.z*x - z*v.x, v.x*y - x*v.y); 
	}
	
	inline virtual bool operator==(const vec3& v) const 
	{ 
		if (x != v.x || y != v.y || z != v.z) return false; 
		return true; 
	}

	inline virtual bool operator!=(const vec3& v) const
	{ 
		if (x != v.x || y != v.y || z != v.z) return true; 
		return false; 
	}

	inline float& operator[](const int idx)
	{
		if (idx == 0) return x; if (idx == 1) return y; return z; 
	}

	inline virtual vec3 scale(const vec3& v) const 
	{ 
		return vec3(x*v.x, y*v.y, z*v.z);
	}

	inline virtual float get(int i) const
	{ 
		if (i == 0) return x; if (i == 1) return y; return z;  
	}

	inline virtual void set(int i, float n)
	{ 
		if (i == 0) x = n;
		else if (i == 1) y = n; 
		else if (i == 2) z = n; 
	}

	inline virtual float dot(const vec3& v) const { return *this*v; }
	inline virtual vec3  cross(const vec3& v) const {return *this^v; }

	inline virtual float len() const { return sqrtf((*this)*(*this)); }
	inline virtual vec3 normalize() const { float ln = len(); if (ln > 0.00001f) return *this/ln; else return vec3(0); }

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

	inline virtual vec3 Inv(bool bx, bool by, bool bz) const
	{ 
		vec3 r = *this; if (bx) r.x = -r.x; if (by) r.y = -r.y; if (bz) r.z = -r.z; return r; 
	} 

	inline virtual vec3 InvX() const { vec3 r = *this; r.x = -r.x; return r; }
	inline virtual vec3 InvY() const { vec3 r = *this; r.y = -r.y; return r; }
	inline virtual vec3 InvZ() const { vec3 r = *this; r.z = -r.z; return r; }
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


struct mat3x3
{
	float m[3][3];

	inline mat3x3()
	{
		m[0][0] = 1;   m[0][1] = 0;   m[0][2] = 0;
		m[1][0] = 0;   m[1][1] = 1;   m[1][2] = 0;
		m[2][0] = 0;   m[2][1] = 0;   m[2][2] = 1;
	};

	inline mat3x3(float mt[3][3])
	{
		m[0][0] = mt[0][0];   m[0][1] = mt[0][1];   m[0][2] = mt[0][2];
		m[1][0] = mt[1][0];   m[1][1] = mt[1][1];   m[1][2] = mt[1][2];
		m[2][0] = mt[2][0];   m[2][1] = mt[2][1];   m[2][2] = mt[2][2];
	}

	inline mat3x3(float m11,float m12,float m13,
		          float m21,float m22,float m23,
				  float m31,float m32,float m33)
	{
		m[0][0] = m11;   m[0][1] = m12;   m[0][2] = m13;
		m[1][0] = m21;   m[1][1] = m22;   m[1][2] = m23;
		m[2][0] = m31;   m[2][1] = m32;   m[2][2] = m33;
	}

	inline void operator=(const mat3x3& mt)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m[i][j] = mt.m[i][j];
	}

	inline mat3x3 operator+(const mat3x3& mt) const
	{
		mat3x3 r;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				r[i][j] = m[i][j] + mt.m[i][j];

		return r;
	}

	inline mat3x3 operator-(const mat3x3& mt) const
	{
		mat3x3 r;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				r[i][j] = m[i][j] + mt.m[i][j];

		return r;
	}

	inline mat3x3 operator*(float v) const
	{
		mat3x3 r;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				r[i][j] = m[i][j]*v;

		return r;
	}

	inline mat3x3 operator/(float v) const
	{ 
		return *this*(1.0f/v); 
	} 

	inline mat3x3 operator*(const mat3x3& mt) const
	{
		mat3x3 r;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				r[i][j] = m[i][0]*mt.m[0][j] + m[i][1]*mt.m[1][j] + m[i][2]*mt.m[2][j];

		return r;
	}

	inline float* operator[](int i) { return m[i]; }

	inline bool operator==(const mat3x3& mt) const
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (m[i][j] !=mt.m[i][j]) return false;

		return true;
	}

	inline mat3x3 rotateAxis(const vec3& axis, float rad) const
	{
		mat3x3 mt;

		vec3 osX = (axis^vec3(1, 1, 1)).normalize();
		vec3 osZ = (osX^axis).normalize()*(-1.0f);
		vec3 osY = axis.normalize();

		mt = mat3x3(osX.x, osX.y, osX.z,
				    osY.x, osY.y, osY.z,
				    osZ.x, osZ.y, osZ.z);

		mat3x3 imt = mt.transpose();

		mat3x3 ret;
		mat3x3 rotate; rotate.SetRotationY(rad);

		ret=(*this)*imt*rotate*mt;

		return ret;
	}

	inline mat3x3 invX() const
	{
		return mat3x3(-m[0][0], -m[0][1], -m[0][2],
			           m[1][0],  m[1][1],  m[1][2],
					   m[2][0],  m[2][1],  m[2][2]);
	}

	inline mat3x3 invY() const
	{
		return mat3x3( m[0][0],  m[0][1],  m[0][2],
			          -m[1][0], -m[1][1], -m[1][2],
					   m[2][0],  m[2][1],  m[2][2]);
	}

	inline mat3x3 invZ() const
	{
		return mat3x3 (m[0][0],  m[0][1],  m[0][2],
			           m[1][0],  m[1][1],  m[1][2],
					  -m[2][0], -m[2][1], -m[2][2]);
	}

	inline mat3x3 transpose() const
	{
		mat3x3 r;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				r[i][j] = m[j][i];

		return r;		
	}

	inline float det() const
	{
		return (m[0][0]*(m[1][1]*m[2][2]-m[1][2]*m[2][1]) -
			    m[0][1]*(m[1][0]*m[2][2]-m[1][2]*m[2][0]) +
			    m[0][2]*(m[1][0]*m[2][1]-m[2][0]*m[1][1]));
	}

	inline mat3x3 inverse() const
	{
		float invdet = 1.0f/det();
		mat3x3 old = *this, ret;   
		ret.m[0][0] =  (old.m[1][1]*old.m[2][2] - old.m[1][2]*old.m[2][1])*invdet;
		ret.m[0][1] = -(old.m[0][1]*old.m[2][2] - old.m[0][2]*old.m[2][1])*invdet;
		ret.m[0][2] =  (old.m[0][1]*old.m[1][2] - old.m[0][2]*old.m[1][1])*invdet;

		ret.m[1][0] = -(old.m[1][0]*old.m[2][2] - old.m[1][2]*old.m[2][0])*invdet;
		ret.m[1][1] =  (old.m[0][0]*old.m[2][2] - old.m[0][2]*old.m[2][0])*invdet;
		ret.m[1][2] = -(old.m[0][0]*old.m[1][2] - old.m[0][2]*old.m[1][0])*invdet;
		 
		ret.m[2][0] =  (old.m[1][0]*old.m[2][1] - old.m[1][1]*old.m[2][0])*invdet;
		ret.m[2][1] = -(old.m[0][0]*old.m[2][1] - old.m[0][1]*old.m[2][0])*invdet;
		ret.m[2][2] =  (old.m[0][0]*old.m[1][1] - old.m[0][1]*old.m[1][0])*invdet;
		
		return ret;
	}

	inline void SetScale(float sx, float sy, float sz)
	{
		*this = mat3x3(sx,0,0,  0,sy,0,  0,0,sz);
	}

	inline void SetScale(vec3 s) { SetScale(s.x,s.y,s.z); }

	inline void SetScale(float scale) { SetScale(scale,scale,scale); }

	inline vec3 transform(const vec3& v) const
	{
		vec3 r;

		r.x = m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z;
		r.y = m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z;
		r.z = m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z;

		return r;
	}

	inline mat3x3 orthonormalize() const
	{
		mat3x3 m = transpose();
		vec3 u1 = vec3(m[1][1], m[1][2], m[1][3]);
		vec3 u2 = vec3(m[2][1], m[2][2], m[2][3]);
		vec3 u3 = vec3(m[3][1], m[3][2], m[3][3]);

		vec3 w1 = (u1).normalize();
		vec3 w2 = (u2 - proj(w1, u2)).normalize();
		vec3 w3 = (u3 - proj(w1, u3) - proj(w2, u3)).normalize();

		mat3x3 r;
		r.m[0][0] = w1.x;	r.m[0][1] = w1.y;	r.m[0][2] = w1.z;
		r.m[1][0] = w2.x;	r.m[1][1] = w2.y;	r.m[1][2] = w2.z;
		r.m[2][0] = w3.x;	r.m[2][1] = w3.y;	r.m[2][2] = w3.z;

		return r;
	}

	inline void SetRotationX(float rad)
	{
		float cs = cosf(rad), sn = sinf(rad);
		*this = mat3x3(1,0,0,  0,cs,sn,  0,-sn,cs);
	}

	inline void SetRotationY(float rad)
	{
		float cs = cosf(rad), sn = sinf(rad);
		*this = mat3x3(cs,0,-sn,   0,1,0,   sn,0,cs);
	}

	inline void SetRotationZ(float rad)
	{
		float cs = cosf(rad), sn = sinf(rad);
		*this = mat3x3(cs,sn,0,  -sn,cs,0,  0,0,1);
	}

	inline void SetRotationXYZ(float rx, float ry, float rz)
	{
		mat3x3 x,y,z;
		x.SetRotationX(rx); y.SetRotationY(ry); z.SetRotationZ(rz);
		*this = x*y*z;
	}

	inline vec3 getXVector() const
	{
		return vec3(m[0][0], m[0][1], m[0][2]);
	}

	inline vec3 getYVector() const
	{
		return vec3(m[1][0], m[1][1], m[1][2]);
	}

	inline vec3 getZVector() const
	{
		return vec3(m[2][0], m[2][1], m[2][2]);
	}
};

inline vec3 operator*(const vec3& v, const mat3x3& m) { return m.transform(v); }
inline vec3 operator*=(const vec3& v, const mat3x3& m) { return m.transform(v); }

struct CollisionPoint
{
	vec3 mPoint;
	vec3 mNormal;
	float mDepth;

	vec3 mImpulse;
	float mBiasImpulse;

	float Kn, Kf1, Kf2, J, Jf1, Jf2, B, Bf1, Bf2, Bias;

	vec3 t1, t2, n1, n2, w1, w2, f1n1, f1n2, f1w1, f1w2, f2n1, f2n2, f2w1, f2w2;

//functions
	CollisionPoint(): Kn(0), Kf1(0), Kf2(0), J(0), Jf1(0), Jf2(0), B(0) {}

	void reset() { Kn = Kf1 = Kf2 = J = Jf1 = Jf2 = B = Bias = mDepth = 0; t1 = t2 = vec3(0); }
};

struct CollisionGeometryVertex
{
	vec3              mLocalPos;
	vec3              mGlobalPos;

	phCollisionPoint* mCollisionPoint;

	CollisionGeometryVertex(const vec3& pos)
	{
		mLocalPos = pos;
		mGlobalPos = pos;
		mCollisionPoint = 0;
	}
};

struct AABB
{
	vec3 mMin, mMax;

	AABB():mMin(0), mMax(0) {}
	AABB(const vec3& minp, const vec3& maxp):mMin(minp), mMax(maxp) {}

	inline bool isIntersect(const AABB& aabb)
	{
		return !(mMin.x > aabb.mMax.x || mMin.y > aabb.mMax.y || mMin.z > aabb.mMax.z ||
			     mMax.x < aabb.mMin.x || mMax.y < aabb.mMin.y || mMax.z < aabb.mMin.z);
	}

	inline bool isIntersect(const vec3& point)
	{
		return !(point.x < mMin.x || point.y < mMin.y || point.z < mMin.z ||
			     point.x > mMax.x || point.y > mMax.y || point.z > mMax.z);
	}

	inline void computeFromPoints(vec3* points, int count)
	{
		mMin = mMax = points[0];
		for (int i = 1; i < count; i++)
		{
			if (points[i].x < mMin.x) mMin.x = points[i].x;
			if (points[i].y < mMin.y) mMin.y = points[i].y;
			if (points[i].z < mMin.z) mMin.z = points[i].z;
			
			if (points[i].x > mMax.x) mMax.x = points[i].x;
			if (points[i].y > mMax.y) mMax.y = points[i].y;
			if (points[i].z > mMax.z) mMax.z = points[i].z;
		}
	}

	inline void reset()
	{
		mMin = vec3(FLT_MAX);
		mMax = vec3(-FLT_MAX);
	}
};

struct lVertex
{
	vec3  mPosition;
	float mFrictionCoef;

	lVertex():mFrictionCoef(1.0f) {}
	lVertex(const vec3& pos, float frictionCoef = 1.0f, float bounceCoef = 1.0f):
		mPosition(pos), mFrictionCoef(frictionCoef) {}
};

struct lPolygon
{
	unsigned int a, b, c;
	lVertex*     pa;
	lVertex*     pb;
	lVertex*     pc;
				     
	vec3         nab, nbc, nca;
	vec3         norm;

	AABB         aabb;

	lPolygon():a(0), b(0), c(0), pa(0), pb(0), pc(0) {}
	lPolygon(unsigned int ia, unsigned int ib, unsigned int ic, lVertex* verticies);

	inline bool isIntersect(const vec3& bottom, vec3* point, vec3* pnorm, float* depth)
	{		
		if ((bottom - pa->mPosition)*nab > 0 ||
			(bottom - pb->mPosition)*nbc > 0 ||
			(bottom - pc->mPosition)*nca > 0)
		{
			return false;
		}

		float ndepth = (pa->mPosition - bottom)*norm;

		if (ndepth < 0)
			return false;

		*pnorm = norm;
		*depth = ndepth;
		*point = bottom - norm*ndepth;

		return true;
	}
};

inline bool IntersectLinePolygon(const vec3& pa, const vec3& pb, const vec3& pc, const vec3& norm, const vec3& la, const vec3& lb, 
	                      vec3 *pt, float* tt /*= NULL*/)
{
	
	vec3 orig = la;

	float ln = (lb - la).len();
	vec3 dir = (lb - la)/ln;

	if (norm*dir < 0) dir *= -1.0f;

	vec3 v0 = pa, 
		 v1 = pb, 
		 v2 = pc;

    vec3 edge1 = v1-v0;
    vec3 edge2 = v2-v0;

    vec3 pvec = dir^edge2;

    float det = edge1*pvec;
    if (fabs(det) < 0.00001f)
        return false;

    vec3 tvec = orig - v0;

    vec3 qvec = tvec^edge1;
    float t = (edge2*qvec)/det;
	
	if (t < 0 || t > ln) 
		return false;

    float u = tvec*pvec;
	if (u < 0 || u > det)
	{
		return false;
	}

    float v = dir*qvec;
    if (v < 0 || u + v > det)
	{
        return false;
	}
	
	*pt = la + dir*t;
	if (tt)
		*tt = t;

	return true;
}

inline mat3x3 rotateMatrixAroundVec(const mat3x3& matr, const vec3& vector, float angle)
{
	vec3 xvec = vector;
	vec3 yvec = xvec^vec3(0, 1, 0);
	if (yvec*yvec < 0.0001f)
		yvec = xvec^vec3(1, 0, 0);

	vec3 zvec = xvec^yvec;

	mat3x3 basicMatrix(xvec.x, xvec.y, xvec.z,
		               yvec.x, yvec.y, yvec.z,
					   zvec.x, zvec.y, zvec.z);

	mat3x3 invBasicMatrix = basicMatrix.transpose();

	mat3x3 rotMatrix; rotMatrix.SetRotationX(angle);

	mat3x3 res = matr*invBasicMatrix*rotMatrix*basicMatrix;
	return res;
}

inline mat3x3 rotatedXMatrix(float angle)
{
	mat3x3 rt;
	rt.SetRotationX(angle);
	return rt;
}

inline mat3x3 rotatedYMatrix(float angle)
{
	mat3x3 rt;
	rt.SetRotationY(angle);
	return rt;
}

inline mat3x3 rotatedZMatrix(float angle)
{
	mat3x3 rt;
	rt.SetRotationZ(angle);
	return rt;
}

inline float rad(float deg) { return deg*0.01745329251994329576f; }
inline float deg(float rad) { return rad*57.2957795130823208767f; }

inline float sign(float v) { if(v>0) return 1.0f; else return -1.0f; }
inline float fmin(float a, float b) { if(a<b) return a; return b; }
inline float fmax(float a, float b) { if(a>b) return a; return b; }

inline int imin(int a, int b) { if(a<b) return a; return b; }
inline int imax(int a, int b) { if(a>b) return a; return b; }
inline void swap(int &a, int &b) { int c=a; a=b; b=c; }

inline float fclamp(float clampValue, float minValue, float maxValue) 
{
	if (clampValue < minValue) return minValue;
	if (clampValue > maxValue) return maxValue;

	return clampValue;
}

inline vec3 vmask(float* vector)
{
	return vec3(vector[0], vector[1], vector[2]);
}

inline void vmask(float* out, const vec3& in)
{
	out[0] = in.x;
	out[1] = in.y;
	out[2] = in.z;
}

inline mat3x3 mmask(float* matrix)
{
	return mat3x3( matrix[0], matrix[1], matrix[2],
		           matrix[3], matrix[4], matrix[5],
				   matrix[6], matrix[7], matrix[8] );
}

inline void mmask(float* out, const mat3x3& in)
{
	out[0] = in.m[0][0]; out[1] = in.m[0][1]; out[2] = in.m[0][2];
	out[3] = in.m[1][0]; out[4] = in.m[1][1]; out[5] = in.m[1][2];
	out[6] = in.m[2][0]; out[7] = in.m[2][1]; out[8] = in.m[2][2];
}

}

#endif //SIMPLE_MATH_H