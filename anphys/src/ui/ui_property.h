#ifndef UI_PROPERTY_H
#define UI_PROPERTY_H

struct uiState;

struct uiProperty
{
	enum InterpolationType { IT_FORCIBLE = 0, IT_LINEAR, IT_SMOOTH };
	enum ChangeState { CS_NONE = 0, CS_ACTIVATING, CS_DEACTIVATING };
	enum OperationType { OP_FORCIBLE_SET = 0, OP_ADDITION, OP_SUBSTRACT, OP_MULTIPLICATION, OP_DIVISION };

	uiState*          mState;

	InterpolationType mInterpolationType;
	float             mCurrentTime;
	float             mDuration;
	float             mPriority;

	ChangeState       mChangeState;

//functions
	uiProperty(InterpolationType interpolationType = IT_LINEAR, float duration = 0.15f, float priority = 1.0f):
		mInterpolationType(interpolationType), mDuration(duration), mCurrentTime(0), mChangeState(CS_NONE), 
		mPriority(priority) {}
	virtual ~uiProperty() {}

	virtual void activate(bool forcible = false) { mChangeState = CS_ACTIVATING; }
	virtual void deactivate(bool forcible = false) { mChangeState = CS_DEACTIVATING; }

	virtual void update(float dt) { mChangeState = CS_NONE; }

	virtual uiProperty* clone() const { return new uiProperty(*this); }
};

template<typename T>
struct uiParameterProperty:public uiProperty
{
	T*            mParameterPtr;

	T             mDisabledValue;
	T             mEnabledValue;
	T             mDifference;
	T             mCurrentValue;

	OperationType mOperationType;
	float         mInvDuration;

	float         mBeginTime;
	float         mEndTime;

	//functions
	uiParameterProperty(T* parameter, const T& disabledValue, const T& enabledvalue,
		                InterpolationType interpolationType = IT_LINEAR, float duration = 0.15f,
		                OperationType operationType = OP_FORCIBLE_SET, float priority = 1.0f):
		uiProperty(interpolationType, duration, priority), mParameterPtr(parameter), mDisabledValue(disabledValue),
		mEnabledValue(enabledvalue), mCurrentValue(disabledValue), mOperationType(operationType) 
	{
		mInvDuration = 1.0f/mDuration;
		mDifference = mEnabledValue - mDisabledValue;
		mBeginTime = 0;
		mEndTime = mBeginTime + mDuration;
	}

	uiParameterProperty(T* parameter, const T& disabledValue, const T& enabledvalue,
		                InterpolationType interpolationType = IT_LINEAR, float beginTime = 0.0f, 
						float duration = 0.15f, float endTime = 0.15f,
		                OperationType operationType = OP_FORCIBLE_SET, float priority = 1.0f):
		uiProperty(interpolationType, duration, priority), mParameterPtr(parameter), mDisabledValue(disabledValue),
		mEnabledValue(enabledvalue), mCurrentValue(disabledValue), mOperationType(operationType) 
	{
		mDifference = mEnabledValue - mDisabledValue;
		mBeginTime = beginTime;
		mEndTime = endTime;

		mDuration = max(mDuration, mEndTime);

		mInvDuration = 1.0f/(mEndTime - mBeginTime);
	}

	uiParameterProperty(const uiParameterProperty<T>& property):uiProperty(property)
	{
		mParameterPtr  = property.mParameterPtr;
		mDisabledValue = property.mDisabledValue;
		mEnabledValue  = property.mEnabledValue;
		mOperationType = property.mOperationType;
		mInvDuration   = property.mInvDuration;
		mDifference    = property.mDifference;
		mBeginTime     = property.mBeginTime;
		mEndTime       = property.mEndTime;
	}

	~uiParameterProperty() {}

	void activate(bool forcible = false)
	{
		if (forcible)
		{
			mCurrentValue = mEnabledValue;
			mCurrentTime = mDuration;
			applyOperation();
			mChangeState = CS_NONE;
		}
		else
		{
			mChangeState = CS_ACTIVATING;
		}
	}

	void deactivate(bool forcible = false)
	{
		if (forcible)
		{
			mCurrentValue = mDisabledValue;
			mCurrentTime = 0.0f;
			applyOperation();
			mChangeState = CS_NONE;
		}
		else
		{
			mChangeState = CS_DEACTIVATING;
		}
	}

	void update(float dt)
	{
		ChangeState prevChangeState = mChangeState;

		if (mChangeState == CS_ACTIVATING)
		{
			mCurrentTime += dt;
			if (mCurrentTime > mEndTime)
			{
				mCurrentTime = mDuration;
				mChangeState = CS_NONE;
			}
		}
		if (mChangeState == CS_DEACTIVATING)
		{
			mCurrentTime -= dt;
			if (mCurrentTime < 0.0f)
			{
				mCurrentTime = 0.0f;
				mChangeState = CS_NONE;
			}
		}

		if (mInterpolationType == IT_LINEAR)
		{
			float coef = fclamp((mCurrentTime - mBeginTime)*mInvDuration, 0.0f, 1.0f);
			mCurrentValue = mDisabledValue + mDifference*coef;
		}
		else if (mInterpolationType == IT_SMOOTH)
		{
			float coef = fclamp((mCurrentTime - mBeginTime)*mInvDuration, 0.0f, 1.0f);
			//sin((x - 0.5)*3.1415926)*0.5 + 0.5
			float smoothCoef = sinf((coef - 0.5f)*3.1415926f)*0.5f + 0.5f;
			mCurrentValue = mDisabledValue + mDifference*smoothCoef;
		}
		else if (mInterpolationType == IT_FORCIBLE)
		{
			/*gLog->fout(1, "Forcible prop %s time %f %.3f-%.3f/%.3f\n", 
				mState->mTargetWidget->mId.c_str(), mCurrentTime, mBeginTime, mEndTime, mDuration);*/
			if (mCurrentTime <= mBeginTime)
				mCurrentValue = mDisabledValue;
			else
				mCurrentValue = mEnabledValue;
		}

		applyOperation();
	}

	void applyOperation()
	{
		if (mOperationType == OP_FORCIBLE_SET)
			*mParameterPtr = mCurrentValue;
		else if (mOperationType == OP_ADDITION)
			*mParameterPtr += mCurrentValue;
		else if (mOperationType == OP_SUBSTRACT)
			*mParameterPtr -= mCurrentValue;
		else if (mOperationType == OP_MULTIPLICATION)
			*mParameterPtr *= mCurrentValue;
		else if (mOperationType == OP_DIVISION)
			*mParameterPtr = *mParameterPtr/mCurrentValue;
	}

	virtual uiProperty* clone() const { return new uiParameterProperty<T>(*this); }
};

#endif //UI_PROPERTY_H