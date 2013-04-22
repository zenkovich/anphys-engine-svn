#ifndef QUAT_H
#define QUAT_H
#include <math.h>

#define MDif 0.00000001f
struct quat
{
	vec3 v;
	float w;

	inline quat() 
	{ 
		v = vec3(0); 
		w = 1.0f; 
	}

	inline quat(float x, float y, float z, float W)
	{ 
		v = vec3(x, y, z); 
		w = W; 
	}

	inline quat(const vec3& V, float W)
	{
		v = V; 
		w = W; 
	}

	inline quat operator+(const quat& q) const 
	{ 
		return quat(v + q.v, w + q.w); 
	}

	inline quat operator-(const quat& q) const
	{ 
		return quat(v - q.v, w - q.w); 
	}

	inline quat operator+=(const quat& q)
	{ 
		*this =* this + q;
		return *this;
	}

	inline quat operator-=(const quat& q) 
	{ 
		*this =* this - q; 
		return *this; 
	}

	inline quat operator*(float s) const
	{ 
		return quat(v*s, w*s); 
	}

	inline quat operator/(float s) const 
	{ 
		float d = 1.0f/s; 
		return *this*d;
	}

	inline quat operator*=(float s) 
	{ 
		*this =* this*s; 
		return *this; 
	}

	inline quat operator/=(float s) 
	{ 
		*this =* this/s;
		return *this;
	}

	inline quat operator*(const quat& q) const 
	{ 
		return quat((v^q.v) + q.v*w + v*q.w, w*q.w - (v*q.v));
	}

	inline quat operator*(const vec3& vt) const 
	{
		return quat(vec3(w*vt.x + v.y*vt.z - v.z*vt.y,
						 w*vt.y + v.z*vt.x - v.x*vt.z,
						 w*vt.z + v.x*vt.y - v.y*vt.x), -(v*vt));
	}

	inline float norm() const 
	{ 
		return v*v + w*w; 
	}

	inline float magnitude() const 
	{
		return sqrtf(norm());
	}

	inline quat conjugate() const 
	{
		return quat(v*-1.0f, w);
	}

	inline quat normalize() const 
	{ 
		return *this/magnitude();
	}

	inline quat inverse() const 
	{ 
		return normalize().conjugate();
	}

	inline void Euler(float yaw, float pitch, float roll)
	{
		quat qroll, qpitch, qyaw;
		qroll.v = vec3(sinf(roll*0.5f), 0, 0); qroll.w = cosf(roll*0.5f);
		qpitch.v = vec3(0, sinf(pitch*0.5f), 0); qpitch.w = cosf(pitch*0.5f);
		qyaw.v = vec3(0, 0, sinf(yaw*0.5f)); qyaw.w = cosf(yaw*0.5f);
		*this = qyaw*qpitch*qroll;
	}
};

#endif
