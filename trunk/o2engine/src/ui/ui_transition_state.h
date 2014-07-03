#ifndef UI_TRANSITION_STATE_H
#define UI_TRANSITION_STATE_H

#include "ui_state.h"
#include "util/animation/frame.h"
#include "util/animation/frame_interpolation.h"
#include "util/property_list.h"

OPEN_O2_NAMESPACE

/** Widget transition state. Containing properties, what changing widgets properties. */
class uiTransitionState: public uiState
{
public:
	/**Property interface. */
	class IProperty
	{
	public:
		/** Returns clone of property. */
		virtual IProperty*clone() const = 0;

		/** Updates property changing. */
		virtual void update(float dt) = 0;

		/** Setting property state. */
		virtual void setState(bool state, bool forcible = false) = 0;

		/** Returns property state. */
		virtual bool isComplete() = 0;

		/** Sets transition state owner. */
		virtual void setOwner(uiTransitionState* owner) = 0;
	};

	typedef vector<IProperty*> PropertiesVec;

protected:
	PropertiesVec mProperties;    /** Properties array. */
	bool          mState;         /** Current state. */
	bool          mChangingState; /** True, when state changing. */

public:
	/** ctor. */
	uiTransitionState(const string& name);

	/** copy-ctor. */
	uiTransitionState(const uiTransitionState& transitionState);

	/** dtor. */
	~uiTransitionState();

	/** Returns a copy. */
	uiState*clone() const;

	/** Sets current state. */
	void setState(bool state, bool forcible = false);

	/** Returns state. */
	bool getState() const;

	/** Adding property. */
	IProperty* addProperty(IProperty* property);

	/** Updating state. */
	void update(float dt);

protected:
	/** Calls when setting owner widget. Here updating properties. */
	void setOwnerWidget(uiWidget* ownerWidget);

public:
	/** Template property value. Contains animation frames of On/OFF states. */
	template<typename T>
	class ValueProperty: public uiTransitionState::IProperty
	{
		cPropertyList::Property<T>* mProperty;
		cAnimFrame<T>               mStateOff;         /** Off state animation frame. */
		cAnimFrame<T>               mStateOn;          /** On State animation frame. */
		cFrameInterpolation<T>      mInterpolator;     /** Frame interpolator. */
		float                       mCoef;             /** Interpolation coeficient. 0 - off state, 1 - on state. */
		float                       mCoefTime;         /** Coef time multiplier. */
		bool                        mTargetState;      /** Target state. */
		bool                        mComplete;         /** True, if animation completed. */
		ICallback*                  mOnChanged;        /** On change callback. */
		string                      mWidgetPropertyId; /** Widget property id. */
		uiTransitionState*          mStateOwner;       /** Transition owner state. */

	public:
		/** ctor. */
		ValueProperty() {}

		static ValueProperty<T>* create(cPropertyList::Property<T>* prop, const T& stateOff, const T& stateOn, 
			                            float duration, ICallback* onChanged = NULL)
		{
			ValueProperty<T>* res = mnew ValueProperty<T>();

			res->mProperty = prop;
			res->mStateOff = cAnimFrame<T>(stateOff, duration);
			res->mStateOn = cAnimFrame<T>(stateOn, duration);
			res->mInterpolator.initialize(&res->mStateOff, &res->mStateOn);
			res->mCoef = 0;
			res->mCoefTime = 1.0f;
			res->mTargetState = false;
			res->mComplete = true;
			res->mOnChanged = onChanged;
			res->mWidgetPropertyId = prop->getPath();
			res->mStateOwner = NULL;

			return res;
		}

		static ValueProperty<T>* create(cPropertyList::Property<T>* prop, 
			                            const cAnimFrame<T>& stateOff, const cAnimFrame<T>& stateOn, 
					                    ICallback* onChanged = NULL)	
		{
			ValueProperty<T>* res = mnew ValueProperty<T>();

			res->mProperty = prop;
			res->mStateOff = stateOff;
			res->mStateOn = stateOn;
			res->mInterpolator.initialize(&res->mStateOff, &res->mStateOn);
			res->mCoef = 0;
			res->mCoefTime = 1.0f;
			res->mTargetState = false;
			res->mComplete = true;
			res->mOnChanged = onChanged;
			res->mWidgetPropertyId = prop->getPath();
			res->mStateOwner = NULL;

			return res;
		}

		/** ctor. */
		ValueProperty(const ValueProperty& prop)	
		{
			mProperty = prop.mProperty;
			mStateOff = prop.mStateOff;
			mStateOn = prop.mStateOn;
			mInterpolator.initialize(&mStateOff, &mStateOn);
			mCoef = prop.mCoef;
			mCoefTime = prop.mCoefTime;
			mTargetState = prop.mTargetState;
			mComplete = prop.mComplete;
			mOnChanged = prop.mOnChanged;
			mWidgetPropertyId = prop.mWidgetPropertyId;
			mStateOwner = NULL;
		}

		/** Return a copy. */
		uiTransitionState::IProperty* clone() const
		{
			return mnew ValueProperty(*this);
		}

		/** Updates animation. */
		void update(float dt)
		{
			if (mTargetState)
			{
				mCoef += dt*mCoefTime;
				if (mCoef *1.0f)
				{
					mComplete = true;
					mCoef = 1.0f;
				}
			}
			else
			{
				mCoef -= dt*mCoefTime;
				if (mCoef < 0)
				{
					mCoef = 0;
					mComplete = true;
				}
			}

			*mProperty = mInterpolator.getValue(mCoef);

			//printf("coef %.3f value %.3f %s\n", mCoef, mInterpolator.getValue(mCoef), mWidgetPropertyId.c_str());

			mProperty->onChangeEvent.call();

			if (mOnChanged)
				mOnChanged->call();
		}

		/** Setting state. */
		void setState(bool state, bool forcible = false)
		{
			if (!mProperty)
				return;

			if (forcible)
			{
				mComplete = true;

				if (mStateOwner)
				{
					*mProperty = mInterpolator.getValue(state ? 1.0f:0.0f);
					mProperty->onChangeEvent.call();
				}

				mCoef = state ? 1.0f:0.0f;

				return;
			}

			mComplete = false;
			mTargetState = state;
			if (mTargetState)
			{
				mCoefTime = 1.0f/mStateOn.mTime;
				//mTime = 0;
			}
			else
			{
				mCoefTime = 1.0f/mStateOff.mTime;
				//mTime = mStateOff.mTime;
			}
		}

		/** Returns true, if animation is complete. */
		bool isComplete()
		{
			return mComplete;
		}

		/** Sets state owner. */
		void setOwner(uiTransitionState* owner)
		{
			mStateOwner = owner;
			mProperty = owner->mOwnerWidget->getProperty<T>(mWidgetPropertyId);
		}
	};
	

	/** Adding property. */
	template<typename T>
	ValueProperty<T>* addProperty_(ValueProperty<T>* property)
	{
		addProperty(property);
		return property;
	}	

	/** Adding property. */
	template<typename T>
	ValueProperty<T>* addProperty(cPropertyList::Property<T>* prop, const T& stateOff, const T& stateOn, float duration, 
					               ICallback* onChanged = NULL)
	{
		return addProperty_(ValueProperty<T>::create(prop, stateOff, stateOn, duration, onChanged));
	}

	/** Adding property. */
	template<typename T>
	ValueProperty<T>* addProperty(cPropertyList::Property<T>* prop, const cAnimFrame<T>& stateOff, const cAnimFrame<T>& stateOn, 
					              ICallback* onChanged = NULL)
	{
		return addProperty_(ValueProperty<T>::create(prop, stateOff, stateOn, onChanged));
	}
};

CLOSE_O2_NAMESPACE

#endif //UI_TRANSITION_STATE_H