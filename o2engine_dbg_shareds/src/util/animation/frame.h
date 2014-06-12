#ifndef FRAME_H
#define FRAME_H

#include "public.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

/** Animation frame. Contains template value, interpolation type, bezier, time and some data. */
template<typename T>
struct cAnimFrame
{
	T                 mValue;    /** Target value. */
	float             mTime;     /** Time to that frame. */
	InterpolationType mType;     /** Type of time interpolation. */
	bool              mBezier;   /** True, if using bezier interpolation. First data bytes is bezier supporting value. */
	float*            mData;     /** Some data. For bezier or curve time interpolation. */
	uint32            mDataSize; /** Size of additive data. */

	/** ctor. */
	cAnimFrame():mTime(0), mDataSize(0), mData(0) {}

	/** ctor. */
	cAnimFrame(const T& value, float time = 1.0f, bool bezier = false, InterpolationType type = IT_LINEAR, 
		       uint32 dataSize = 0, float* data = NULL):
		mValue(value), mTime(time), mType(type), mData(data), mDataSize(dataSize), mBezier(bezier) {}

	/** copy-ctor. */
	cAnimFrame(const cAnimFrame& frm)
	{
		mValue = frm.mValue;
		mTime = frm.mTime;
		mType = frm.mType;
		mBezier = frm.mBezier;
		mDataSize = frm.mDataSize;
		mData = NULL;

		if (frm.mDataSize > 0)
		{
			mData = mnew float[mDataSize];
			memcpy(mData, frm.mData, mDataSize);
		}
	}

	/** dtor. */
	~cAnimFrame()
	{
		safe_release(mData);
	}

	cAnimFrame& operator=(const cAnimFrame& frm)
	{
		safe_release(mData);

		mValue = frm.mValue;
		mTime = frm.mTime;
		mType = frm.mType;
		mBezier = frm.mBezier;
		mDataSize = frm.mDataSize;
		mData = NULL;

		if (frm.mDataSize > 0)
		{
			mData = mnew float[mDataSize];
			memcpy(mData, frm.mData, mDataSize);
		}

		return *this;
	}
};

CLOSE_O2_NAMESPACE

#endif // FRAME_H
