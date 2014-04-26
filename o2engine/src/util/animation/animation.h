#ifndef ANIMATION_H
#define ANIMATION_H

#include "animation_base.h"
#include "frame.h"
#include "frame_interpolation.h"
#include "public.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

/** Animated parameter. */
template<typename T>
class cAnimation: virtual public IAnimation
{
	friend class AnimationTest;
	typedef vector< cAnimFrame<T> > FramesVec;

public:
	
protected:
	T         mValue;     /**< Animating value. */
	T         mLastValue; /**< Last value. */
	T*        mBindValue; /**< Pointer to binded value. */
	FramesVec mFrames;    /**< Frames vector. */
			  
	int       mCurrentFrame;             /**< Index of current frame. */
	float     mCurrentFrameBeginTime;    /**< Current frame begin time. */
	float     mCurrentFrameEndTime;      /**< Current frame end time. */
	float     mCurrentFrameDuration;     /**< Duration of current frame. */
	float     mCurrentFrameInvDuration;  /**< Inverted duration of current frame. */

	cFrameInterpolation<T> mFrameInterp; /**< Frames interpolator. */

public:

	/** ctor. */
	cAnimation(int frames = 0, float duration = 1.0f): IAnimation(),
		mCurrentFrame(0), mCurrentFrameBeginTime(0), mCurrentFrameEndTime(0), mCurrentFrameDuration(0),
		mCurrentFrameInvDuration(1)
	{
		mBindValue = &mValue;

		float frameDuration = (frames == 0) ? 0.0f:duration/(float)frames;
		for (int i = 0; i < frames; i++)
			mFrames.push_back(cAnimFrame<T>(mValue, frameDuration));

		mDuration = frames*frameDuration;
	}

	/** dtor. */
	virtual ~cAnimation() {}

	/** Access operator. */
	T& operator*() { return mValue; }

	/** Access operator. */
	T& operator->() { return mValue; }

	/** Bind value. */
	void bindValue(T* valuePtr) 
	{
		mBindValue = valuePtr;
	}

	/** Returns true, if value changed from last frame. */
	bool isChanged() const
	{
		return mValue != mLastValue;
	}

	/** Add frame. */
	int addFrame(const T& value, int position = -1, float time = 1.0f, bool bezier = false, 
		         InterpolationType type = IT_LINEAR, uint32 dataSize = 0, float* data = NULL)
	{
		return addFrame(cAnimFrame<T>(value, time, bezier, type, dataSize, data), position);
	}

	/** Add frame. */
	int addFrame(const cAnimFrame<T>& frm, int position = -1) 
	{
		if (mFrames.size() == 0)
		{
			mDuration -= frm.mTime;
		}

		if (position < 0)
		{
			mFrames.push_back(frm);
			mDuration += frm.mTime;

			if (mFrames.size() == 2)
				firstInitialize();

			return mFrames.size() - 1;
		}

		mFrames.insert(mFrames.begin() + position, frm);
		mDuration += frm.mTime;

		if (mFrames.size() == 2)
			firstInitialize();

		return position;
	}

	/** Removing frame. */
	void removeFrame(int position)
	{
		if (mFrames.size() < 1)
			return;

		int idx = clamp(position, 0, mFrames.size() - 1);
		mDuration -= mFrames[idx].mTime;
		mFrames.erase(mFrames.begin() + idx);
	}

	/** Removing all frames. */
	void removeAllFrames()
	{
		mDuration = 0;
		mFrames.clear();
	}

	/** Returns frames vector. */
	const FramesVec& getFrames() const 
	{
		return mFrames;
	}

protected:
	/** Updating value. */
	void evaluate()
	{
		seekFrame();

		float timeCoef = mCurrentFrameInvDuration*(mTime - mCurrentFrameBeginTime);

		mFrameInterp.getValue(mValue, timeCoef);
		
		mLastValue = mValue;
		*mBindValue = mValue;
	}

	/** Seek frame by local time. */
	void seekFrame() 
	{		
		bool x = true;
		while (mTime < mCurrentFrameBeginTime && mCurrentFrame > 1)
		{
			mCurrentFrame--;
			cAnimFrame<T>* frm = &mFrames[mCurrentFrame];
			mCurrentFrameEndTime = mCurrentFrameBeginTime;
			mCurrentFrameBeginTime -= frm->mTime;
			
			updateFrameSupportValues();
			x = false;
		} 
		
		while (x && mTime > mCurrentFrameEndTime && mCurrentFrame < (int)mFrames.size() - 1)
		{
			mCurrentFrame++;
			cAnimFrame<T>* frm = &mFrames[mCurrentFrame];
			mCurrentFrameBeginTime = mCurrentFrameEndTime;
			mCurrentFrameEndTime += frm->mTime;
			updateFrameSupportValues();
		} 
	}

	/** Updates support values for current frame, initializes frame interpolator. */
	void updateFrameSupportValues()
	{
		mFrameInterp.initialize(&mFrames[mCurrentFrame - 1], &mFrames[mCurrentFrame]);

		mCurrentFrameDuration = mCurrentFrameEndTime - mCurrentFrameBeginTime;
		mCurrentFrameInvDuration = 1.0f/mCurrentFrameDuration;
	}

	/** First frame initializations. */
	void firstInitialize() 
	{
		mCurrentFrameBeginTime = 0;
		mCurrentFrameEndTime = mFrames[1].mTime;
		mCurrentFrame = 1;
		updateFrameSupportValues();
	}
};

CLOSE_O2_NAMESPACE

#endif //ANIMATION_H