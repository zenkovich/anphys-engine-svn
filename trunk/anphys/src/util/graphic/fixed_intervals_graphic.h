#ifndef FIXED_INTERVALS_GRAPHIC_H
#define FIXED_INTERVALS_GRAPHIC_H

#include "util/memory/mem_utils.h"
#include <math.h>

#define clamp(v, minv, maxv) { if (v < minv) return minv; if (v > maxv) return maxv; return v; }

struct cFixedIntervalsGraphic
{
	struct CoefChain
	{
		float*       mCoeficients;
		unsigned int mCoeficientsCount;

		CoefChain(unsigned int coefsCount)
		{
			mCoeficientsCount = coefsCount;
			mCoeficients = new float[mCoeficientsCount];
		}
		~CoefChain()
		{
			safe_release_arr(mCoeficients);
		}
	};

	struct Value
	{
		float mValue;

		float mMinRange;
		float mMaxRange;
		float mInterval;

		Value*       mValues;
		unsigned int mValuesCount;

		Value()
		{
			mValuesCount = 0;
		}

		Value(float value)
		{
			create(value);
		}

		Value(unsigned int valuesCount)
		{
			create(valuesCount);
		}

		void create(unsigned int valuesCount)
		{
			mValuesCount = valuesCount;
			mValues = new Value[mValuesCount];
		}

		void create(float value)
		{
			mValue = value;
			mValuesCount = 0;
		}

		float getValue(CoefChain& coefChain, unsigned int coefIdx)
		{
			if (mValuesCount == 0)
				return mValue;

			float coef = coefChain.mCoeficients[coefIdx];

			unsigned int leftValueIdx = clamp((coef - mMinRange)/mInterval, mValuesCount - 2);
			unsigned int rightValueIdx = leftValue + 1;
			
			float leftValue = mValues[leftValueIdx].getValue(coefChain, coefIdx + 1);
			float rightValue = mValues[rightValueIdx].getValue(coefChain, coefIdx + 1);
			
			float leftValueCoef = (float)leftValueIdx*mInterval + mMinRange;
			float rightValueCoef = leftValueCoef;

			float intervalCoef = (coef - leftValueCoef)/mInterval;

			float res = leftValue + (rightValue - leftValue)*intervalCoef;
		}
	};

	Value        mData;
	unsigned int mCoeficientsCount;

	cFixedIntervalsGraphic()
	{
		mCoeficientsCount = 0;
	}

	cFixedIntervalsGraphic(unsigned int coeficientsCount)
	{
		create(coeficientsCount);
	}

	void create(unsigned int coeficientsCount)
	{
		mCoeficientsCount = coeficientsCount;
	}

	CoefChain* createCoefChain()
	{
		return new CoefChain(mCoeficientsCount);
	}

	float getValue(CoefChain& coefChain)
	{
		return mData.getValue(coefChain, 0);
	}
};

#endif //FIXED_INTERVALS_GRAPHIC_H