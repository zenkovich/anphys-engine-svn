#ifndef BASIS_H
#define BASIS_H

#include "public.h"
#include "vector2.h"

OPEN_O2_NAMESPACE

/** Basis. Just as matrix 2x3. */
struct basis
{
	vec2f xv, yv, offs;

	inline basis():
		xv(1, 0), yv(0, 1), offs()
	{
	}

	inline basis(const vec2f& offsvec, const vec2f& xvec = vec2f(1, 0), const vec2f& yvec = vec2f(0, 1))
	{
		xv = xvec; yv = yvec;
		offs = offsvec;
	}

	inline basis(const vec2f& offsvec, float angle)
	{
		offs = offsvec;
		float cs = cosf(angle), sn = sinf(angle);
		xv.set(cs, sn);
		yv.set(-sn, cs);
	}

	inline void set(const vec2f& offsvec = vec2f(0, 0), const vec2f& xvec = vec2f(1, 0), const vec2f& yvec = vec2f(0, 1))
	{
		xv = xvec; yv = yvec;
		offs = offsvec;
	}

	inline void set(const vec2f& offsvec, float angle)
	{
		offs = offsvec;
		float cs = cosf(angle), sn = sinf(angle);
		xv.set(cs, sn);
		yv.set(-sn, cs);
	}

	inline basis operator=(const basis& cbasis)
	{
		offs = cbasis.offs;
		xv = cbasis.xv;
		yv = cbasis.yv;
		return *this;
	}

	inline bool operator==(const basis& cbasis)
	{
		return !(xv != cbasis.xv || yv != cbasis.yv || offs != cbasis.offs);
	}

	inline bool operator!=(const basis& cbasis)
	{
		return xv != cbasis.xv || yv != cbasis.yv || offs != cbasis.offs;
	}

	inline basis operator*(const basis& cbasis)
	{ 
		basis res;
		res.xv.x = xv.x*cbasis.xv.x + xv.y*cbasis.yv.x;                       res.xv.y = xv.x*cbasis.xv.y + xv.y*cbasis.yv.y;
		res.yv.x = yv.x*cbasis.xv.x + yv.y*cbasis.yv.x;                       res.yv.y = yv.x*cbasis.xv.y + yv.y*cbasis.yv.y;
		res.offs.x = offs.x*cbasis.xv.x + offs.y*cbasis.yv.x + cbasis.offs.x; res.offs.y = offs.x*cbasis.xv.y + offs.y*cbasis.yv.y + cbasis.offs.y;
		return res;
	}

	inline vec2f operator*(const vec2f& vec) const
	{
		vec2f ret;
		ret.x = xv.x*vec.x + yv.x*vec.y + offs.x;
		ret.y = xv.y*vec.x + yv.y*vec.y + offs.y;
		return ret;
	}

	inline float getAngle() const
	{
		float angle = -atan2f( -xv.y, xv.x );
		if (angle < 0)
			return 6.283185307f + angle;
		
		return angle;
	}

	inline vec2f getScale() const
	{
		return vec2f( xv.len(), yv.len() );
	}

	inline float getShift() const 
	{
		vec2f scale = getScale();
		return getShiftFast(scale);
	}

	inline float getShiftFast(const vec2f& scale) const 
	{
		return (xv/scale.x)*(yv/scale.y);
	}

	inline void decompose(vec2f* offset, float* angle, vec2f* scale, float* shift) const
	{
		*offset = offs;
		*angle = getAngle();
		*scale = getScale();
		*shift = getShiftFast(*scale);
	}

	inline basis inverted() const
	{
		float invdet = 1.0f/(xv.x*yv.y - yv.x*xv.y);
		basis res;   

		res.xv.x=     yv.y*invdet;
		res.yv.x=    -yv.x*invdet;
		res.offs.x=  (yv.x*offs.y - offs.x*yv.y)*invdet;
				    
		res.xv.y=    -xv.y*invdet;
		res.yv.y=     xv.x*invdet;
		res.offs.y= -(xv.x*offs.y - offs.x*xv.y)*invdet;
		
		return res;
	}

	inline void inverse()
	{
		*this = inverted();
	}

	inline void translate(const vec2f& voffs)
	{
		offs += voffs;
	}

	inline void scale(const vec2f& scalev)
	{
		xv *= scalev.x;
		yv *= scalev.y;
	}

	inline void rotate(float angle)
	{
		float cs = cosf(angle), sn = sinf(angle);
		
		vec2f nxv( cs*xv.x - sn*xv.y, sn*xv.x + cs*xv.y );
		vec2f nyv( cs*yv.x - sn*yv.y, sn*yv.x + cs*yv.y );

		xv = nxv; yv = nyv;
	}

	inline void transform(float& x, float& y) const
	{
		float lx = x, ly = y;
		x = xv.x*lx + yv.x*ly + offs.x;
		y = xv.y*lx + yv.y*ly + offs.y;
	}

	inline vec2f transform(const vec2f& vec) const
	{
		return vec2f(xv.x*vec.x + yv.x*vec.y + offs.x, xv.y*vec.x + yv.y*vec.y + offs.y);
	}

	inline static basis nullBasis()
	{
		return basis(vec2f(0, 0), vec2f(1, 0), vec2f(0, 1));
	}

	inline static basis scaledBasis(const vec2f& scale)
	{
		return basis(vec2f(0, 0), vec2f(scale.x, 0), vec2f(0, scale.y));
	}

	inline static basis translatedBasis(const vec2f& voffs)
	{
		return basis(voffs, vec2f(1, 0), vec2f(0, 1));
	}

	inline static basis rotatedBasis(float angle)
	{
		float cs = cosf(angle), sn = sinf(angle);
		basis(vec2f(0, 0), vec2f(cs, sn), vec2f(-sn, cs));
	}

	inline static basis buildBasis(const vec2f& position, const vec2f& scale, float angle, float shift)
	{
		float sn = sinf(angle), cs = cosf(angle);
		vec2f x(scale.x*cs, sn*scale.x), y(-sn*scale.y, cs*scale.y);
		y += x*shift;
		return basis(position, x, y);
	}
};

struct basisDef 
{
	vec2f mPosition;
	vec2f mScale;
	float mAngle;
	float mShift;


	inline basisDef(const vec2f& position = vec2f(), const vec2f& scale = vec2f(1, 1), float angle = 0, float shift = 0):
		mPosition(position), mScale(scale), mAngle(angle), mShift(shift)
	{
	}

	inline basisDef(const basisDef& def)
	{
		mPosition = def.mPosition;
		mScale = def.mScale;
		mAngle = def.mAngle;
		mShift = def.mShift;
	}

	inline basisDef(const basis& bas)
	{
		bas.decompose(&mPosition, &mAngle, &mScale, &mShift);
	}

	inline basis build() const 
	{
		return basis::buildBasis(mPosition, mScale, mAngle, mShift);
	}
};

inline basisDef basis2def(const basis& bas)
{
	return basisDef(bas);
}

inline basis def2basis(const basisDef& def)
{
	return def.build();
}

CLOSE_O2_NAMESPACE

#endif //MATRIX3_H