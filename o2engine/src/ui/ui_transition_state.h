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
		virtual shared<IProperty> clone() const = 0;

		/** Updates property changing. */
		virtual void update(float dt) = 0;

		/** Setting property state. */
		virtual void setState(bool state, bool forcible = false) = 0;

		/** Returns property state. */
		virtual bool isComplete() = 0;

		/** Sets transition state owner. */
		virtual void setOwner(const shared<uiTransitionState>& owner) = 0;
	};

	typedef vector< shared<IProperty> > PropertiesVec;

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
	shared<uiState> clone() const;

	/** Sets current state. */
	void setState(bool state, bool forcible = false);

	/** Returns state. */
	bool getState() const;

	/** Adding property. */
	shared<IProperty> addProperty(const shared<IProperty>& property);

	/** Updating state. */
	void update(float dt);

protected:
	/** Calls when setting owner widget. Here updating properties. */
	void setOwnerWidget(const shared<uiWidget>& ownerWidget);

public:
	/** Template property value. Contains animation frames of On/OFF states. */
	template<typename T>
	class ValueProperty: public uiTransitionState::IProperty
	{
		typedef cObjectWithPropertyList::DataProperty<T> Prop;

		shared<Prop>              mProperty;
		cAnimFrame<T>             mStateOff;         /** Off state animation frame. */
		cAnimFrame<T>             mStateOn;          /** On State animation frame. */
		cFrameInterpolation<T>    mInterpolator;     /** Frame interpolator. */
		float                     mTime;             /** Interpolation time. */
		float                     mInvDuration;      /** Inverted animation duration. */
		bool                      mTargetState;      /** Target state. */
		bool                      mComplete;         /** True, if animation completed. */
		shared<ICallback>         mOnChanged;        /** On change callback. */
		string                    mWidgetPropertyId; /** Widget property id. */
		shared<uiTransitionState> mStateOwner;       /** Transition owner state. */

	public:
		/** ctor. */
		ValueProperty(const std::string& propertyId, const T& stateOff, const T& stateOn, float duration, 
					  const shared<ICallback>& onChanged = NULL)
		{
			mProperty = NULL;
			mStateOff = cAnimFrame<T>(stateOff, duration);
			mStateOn = cAnimFrame<T>(stateOn, duration);
			mInterpolator.initialize(&mStateOff, &mStateOn);
			mTime = 0;
			mInvDuration = 1.0f;
			mTargetState = false;
			mComplete = true;
			mOnChanged = onChanged;
			mWidgetPropertyId = propertyId;
			mStateOwner = NULL;
		}

		/** ctor. */
		ValueProperty(const std::string& propertyId, const cAnimFrame<T>& stateOff, const cAnimFrame<T>& stateOn, 
					  const shared<ICallback>& onChanged = NULL)	
		{
			mProperty = NULL;
			mStateOff = stateOff;
			mStateOn = stateOn;
			mInterpolator.initialize(&mStateOff, &mStateOn);
			mTime = 0;
			mInvDuration = 1.0f;
			mTargetState = false;
			mComplete = true;
			mOnChanged = onChanged;
			mWidgetPropertyId = propertyId;
			mStateOwner = NULL;
		}

		/** Return a copy. */
		shared<uiTransitionState::IProperty> clone() const
		{
			shared<ValueProperty> res = mnew ValueProperty(mWidgetPropertyId, mStateOff, mStateOn, mOnChanged);
			return res;
		}

		/** Updates animation. */
		void update(float dt)
		{
			if (mTargetState)
			{
				mTime += dt;
				if (mTime > mStateOn.mTime)
				{
					mComplete = true;
					mTime = mStateOn.mTime;
				}
			}
			else
			{
				mTime -= dt;
				if (mTime < 0)
				{
					mTime = 0;
					mComplete = true;
				}
			}

			*mProperty = mInterpolator.getValue(mTime*mInvDuration);

			mProperty->mOnChange.call();

			if (mOnChanged)
				mOnChanged->call();
		}

		/** Setting state. */
		void setState(bool state, bool forcible = false)
		{
			if (forcible)
			{
				mComplete = true;

				if (mStateOwner)
				{
					*mProperty = mInterpolator.getValue(state ? 1.0f:0.0f);
					mProperty->mOnChange.call();
				}

				mTime = state ? mStateOn.mTime:0.0f;

				return;
			}

			mComplete = false;
			mTargetState = state;
			if (mTargetState)
			{
				mInvDuration = 1.0f/mStateOn.mTime;
				//mTime = 0;
			}
			else
			{
				mInvDuration = 1.0f/mStateOff.mTime;
				//mTime = mStateOff.mTime;
			}
		}

		/** Returns true, if animation is complete. */
		bool isComplete()
		{
			return mComplete;
		}

		/** Sets state owner. */
		void setOwner(const shared<uiTransitionState>& owner)
		{
			mStateOwner = owner;
			mProperty = owner->mOwnerWidget->getProperty<T>(mWidgetPropertyId);
		}
	};
	

	/** Adding property. */
	template<typename T>
	shared< ValueProperty<T> > addProperty(const shared< ValueProperty<T> >& property)
	{
		addProperty(property);
		return property;
	}	

	/** Adding property. */
	template<typename T>
	shared< ValueProperty<T> > addProperty(const std::string& propertyId, const T& stateOff, const T& stateOn, float duration, 
					                       const shared<ICallback>& onChanged = NULL)
	{
		return addProperty(mnew ValueProperty<T>(propertyId, stateOff, stateOn, duration, onChanged));
	}

	/** Adding property. */
	template<typename T>
	shared< ValueProperty<T> > addProperty(const std::string& propertyId, const cAnimFrame<T>& stateOff, const cAnimFrame<T>& stateOn, 
					                       const shared<ICallback>& onChanged = NULL)
	{
		return addProperty(mnew ValueProperty<T>(propertyId, stateOff, stateOn, onChanged));
	}
};

CLOSE_O2_NAMESPACE

#endif //UI_TRANSITION_STATE_H
