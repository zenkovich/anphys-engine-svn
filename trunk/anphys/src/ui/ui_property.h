#ifndef UI_PROPERTY_H
#define UI_PROPERTY_H

struct uiState;

struct uiProperty
{
	enum InterpolationType { IT_FORCIBLE = 0, IT_LINEAR };
	enum ChangeState { CS_NONE = 0, CS_ACTIVATING, CS_DEACTIVATING };

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
	enum OperationType { OP_FORCIBLE_SET = 0, OP_ADDITION, OP_SUBSTRACT, OP_MULTIPLICATION, OP_DIVISION };

	T*            mParameterPtr;

	T             mDisabledValue;
	T             mEnabledvalue;
	T             mDifference;
	T             mCurrentValue;

	OperationType mOperationType;
	float         mInvDuration;

//functions
	uiParameterProperty(T* parameter, const T& disabledValue, const T& enabledvalue,
		                InterpolationType interpolationType = IT_LINEAR, float duration = 0.15f,
						OperationType operationType = OP_FORCIBLE_SET, float priority = 1.0f):
		uiProperty(interpolationType, duration, priority), mParameterPtr(parameter), mDisabledValue(disabledValue),
		mEnabledvalue(enabledvalue), mCurrentValue(disabledValue), mOperationType(operationType) 
	{
		mInvDuration = 1.0f/mDuration;
		mDifference = mEnabledvalue - mDisabledValue;
	}

	uiParameterProperty(const uiParameterProperty<T>& property):uiProperty(property)
	{
		mParameterPtr  = property.mParameterPtr;
		mDisabledValue = property.mDisabledValue;
		mEnabledvalue  = property.mEnabledvalue;
		mOperationType = property.mOperationType;
		mInvDuration   = property.mInvDuration;
		mDifference    = property.mDifference;
	}

	~uiParameterProperty() {}

	void activate(bool forcible = false)
	{
		if (forcible)
		{
			mCurrentValue = mEnabledvalue;
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
		if (mChangeState == CS_ACTIVATING)
		{
			mCurrentTime += dt;
			if (mCurrentTime > mDuration || mInterpolationType == uiProperty::IT_FORCIBLE)
			{
				mCurrentTime = mDuration;
				mChangeState = CS_NONE;
			}
		}
		else if (mChangeState == CS_DEACTIVATING)
		{
			mCurrentTime -= dt;
			if (mCurrentTime < 0.0f || mInterpolationType == uiProperty::IT_FORCIBLE)
			{
				mCurrentTime = 0;
				mChangeState = CS_NONE;
			}
		}

		float coef = mCurrentTime*mInvDuration;
		mCurrentValue = mDisabledValue + mDifference*coef;

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