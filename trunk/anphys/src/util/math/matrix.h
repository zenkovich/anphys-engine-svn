#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>

#define MDif 0.00000001f

//матрица 3х3
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
};

inline mat3x3 m(float f[9]) { return mat3x3(f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7], f[8]); }

inline void   m(mat3x3 mt, float *f) {  f[0] = mt[0][0];   f[1] = mt[0][1];   f[2] = mt[0][2];
										f[3] = mt[1][0];   f[4] = mt[1][1];   f[5] = mt[1][2];
										f[6] = mt[2][0];   f[7] = mt[2][1];   f[8] = mt[2][2];}


struct mat4x4
{
	float m[4][4];

	inline mat4x4()
	{
		m[0][0] = 1;   m[0][1] = 0;   m[0][2] = 0;   m[0][3] = 0;
		m[1][0] = 0;   m[1][1] = 1;   m[1][2] = 0;   m[1][3] = 0;
		m[2][0] = 0;   m[2][1] = 0;   m[2][2] = 1;   m[2][3] = 0;
		m[3][0] = 0;   m[3][1] = 0;   m[3][2] = 0;   m[3][3] = 1;
	};

	inline mat4x4(float mt[4][4])
	{
		m[0][0] = mt[0][0];   m[0][1] = mt[0][1];   m[0][2] = mt[0][2];   m[0][3] = mt[0][3];
		m[1][0] = mt[1][0];   m[1][1] = mt[1][1];   m[1][2] = mt[1][2];   m[1][3] = mt[1][3];
		m[2][0] = mt[2][0];   m[2][1] = mt[2][1];   m[2][2] = mt[2][2];   m[2][3] = mt[2][3];
		m[3][0] = mt[3][0];   m[3][1] = mt[3][1];   m[3][2] = mt[3][2];   m[3][3] = mt[3][3];
	}

	inline mat4x4(float m11,float m12,float m13,float m14,
		          float m21,float m22,float m23,float m24,
				  float m31,float m32,float m33,float m34,
				  float m41,float m42,float m43,float m44)
	{
		m[0][0] = m11;   m[0][1] = m12;   m[0][2] = m13;   m[0][3] = m14;
		m[1][0] = m21;   m[1][1] = m22;   m[1][2] = m23;   m[1][3] = m24;
		m[2][0] = m31;   m[2][1] = m32;   m[2][2] = m33;   m[2][3] = m34;
		m[3][0] = m41;   m[3][1] = m42;   m[3][2] = m43;   m[3][3] = m44;
	}

	inline void operator=(const mat4x4& mt)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = mt.m[i][j];
	}

	inline mat4x4 operator+(const mat4x4& mt) const
	{
		mat4x4 r;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				r[i][j] = m[i][j] + mt.m[i][j];

		return r;
	}

	inline mat4x4 operator-(const mat4x4& mt) const
	{
		mat4x4 r;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				r[i][j] = m[i][j] + mt.m[i][j];

		return r;
	}

	inline mat4x4 operator*(float v) const
	{
		mat4x4 r;

		for (int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				r[i][j] = m[i][j]*v;

		return r;
	}

	inline mat4x4 operator/(float v) const
	{
		return *this*(1.0f/v); 
	} 

	inline mat4x4 operator*(const mat4x4& mt) const
	{
		mat4x4 r;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				r[i][j] = m[i][0]*mt.m[0][j] + m[i][1]*mt.m[1][j] + m[i][2]*mt.m[2][j] + m[i][3]*mt.m[3][j];

		return r;
	}

	inline float* operator[](int i) { return m[i]; }

	inline bool operator==(const mat4x4& mt) const
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (m[i][j] != mt.m[i][j]) return false;

		return true;
	}

	inline mat4x4 transpose() const
	{
		mat4x4 r;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				r[i][j] = m[j][i];

		return r;		
	}

	inline void Identity()
	{
		mat4x4(1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1);
	}

	inline void SetScale(float sx, float sy, float sz)
	{
		mat4x4(sx,0,0,0,  0,sy,0,0,  0,0,sz,0,  0,0,0,1);
	}

	inline void SetScale(vec3& s) { SetScale(s.x, s.y, s.z); }

	inline void SetScale(float scale) { SetScale(scale, scale, scale); }

	inline vec3 transform(const vec3& v) const
	{
		vec3 r;

		r.x = m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0];
		r.y = m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1];
		r.z = m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2];

		return r;
	}

	inline void SetRotationX(float rad)
	{
		float cs = cosf(rad), sn = sinf(rad);
		*this = mat4x4 (1,0,0,0,
			            0,cs,sn,0,
					    0,-sn,cs,0,
					    0,0,0,1);
	}

	inline void SetRotationY(float rad)
	{
		float cs = cosf(rad), sn = sinf(rad);
		*this = mat4x4(cs,0,-sn,0,
			           0,1,0,0,
					   sn,0,cs,0,
					   0,0,0,1);
	}

	inline void SetRotationZ(float rad)
	{
		float cs = cosf(rad), sn = sinf(rad);
		*this = mat4x4(cs,sn,0,0,
			           -sn,cs,0,0,
					   0,0,1,0,
					   0,0,0,1);
	}

	inline void SetRotationXYZ(float rx, float ry, float rz)
	{
		mat4x4 x, y, z;
		x.SetRotationX(rx); y.SetRotationY(ry); z.SetRotationZ(rz);
		*this = x*y*z;
	}
};

#endif
