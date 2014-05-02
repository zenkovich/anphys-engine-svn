#ifndef ANIMATION_BASE_H
#define ANIMATION_BASE_H

#include "public.h"

#include "util/objects.h"
#include "util/callback.h"
#include "util/smart_ptrs.h"


OPEN_O2_NAMESPACE

/** Basic animation structure. Controlling time with looping and intervals. */
class IAnimation:public virtual IDynamic
{
public:
	enum LoopType { LT_NONE = 0, LT_REPEAT, LT_TOGGLE };

protected:

	/** Callback by time. */
	struct TimeCallback
	{
		float               mTime;     /**< Time, when callback will be invoke. */
		autoPtr(ICallback) mCallback; /**< Callback. */

		/** ctor. */
		TimeCallback(sharedPtr(ICallback) cb, float time):mTime(time), mCallback(cb) {}
	};
	typedef vector<TimeCallback> TimeCallbacksVec;

	autoPtr(ICallback) mBeginPlayingCallback; /**< Callback, what calls when animation starting, independ of time. */
	autoPtr(ICallback) mEndPlayingCallback;   /**< Callback, what calls when animation finished, independ of time. */
	TimeCallbacksVec    mTimedCallbacks;       /**< Callback, what calls by time. */
	
	float     mTime;           /**< Local time, in [0...mDuration].*/
	float     mLastFrameTime;  /**< Last local time. Fo calling callbacks by time. */
	float     mAbsoluteTime;   /**< Absolute time. */
	float     mAnimationSpeed; /**< Animation speed coeficient. 1 - standart coef, less - slower animation. */
	float     mDirectionSign;  /**< Is -1 if animation playing reversed. */
	LoopType  mLoop;           /**< Loop type. */
	float     mDuration;       /**< Duration of local time animation. */
	float     mBeginPlaying;   /**< Absolute begin playing time. */
	float     mEndPlaying;     /**< Absolute end playing time. */
	bool      mPlaying;        /**< True, when playing. */
	bool      mPlayed;         /**< True, when local time was expected. */

public:
	/** ctor. */
	IAnimation();

	/** dtor. */
	virtual ~IAnimation();

	/** Updating animation. */
	virtual void update(float dt);

	/** Playing animation. */
	virtual void play(bool restart = false, bool stopAtEnd = true);

	/** Play in interval. */
	virtual void play(float begin, float end);

	/** Stop animation. */
	virtual void stop();

	/** Return current absolute time. */
	virtual float getTime() const;

	/** Returns current local time. */
	virtual float getLocalTime() const;

	/** Setting absolutee time. */
	virtual void setTime(float time);

	/** Returns duration of local animation. */
	virtual float getDuration() const;

	/** Returns true, if animation playing. */
	virtual bool isPlaying() const;

	/** Returns true, if local animation was played. */
	virtual bool isPlayed() const;

	/** Setting loop type. */
	virtual void setLoop(LoopType type = LT_REPEAT);

	/** Setting speed of animation (0...1...inf). */
	virtual void setSpeed(float speed = 1);

	/** Returns speed of animation. */
	virtual float getSpeed() const;

	/** Returns begin playing time. */
	virtual float getBeginTime() const;

	/** Returns end playing time. */
	virtual float getEndTime() const;

	/** Setting playing flag. */
	virtual void setPlaying(bool playing);

	/** Setting begin callback. */
	virtual void setBeginPlayingCallback(sharedPtr(ICallback) cb);

	/** Setting end playing callback. */
	virtual void setEndPlayingCallback(sharedPtr(ICallback) cb);

	/** Adding time callback. */
	virtual void setTimeCallback(float time, sharedPtr(ICallback) cb);

protected:
	/** Invoke begin callback, if exist. */
	void checkBeginCallback();
	
	/** Invoke end callback, if exist. */
	void checkEndCallback();

	/** Invoke callbacks in time interval. */
	void checkTimeCallbacks(float begin, float end);

	/** Update local time (mTime). */
	void updateTime();

	/** Change some data here. */
	virtual void evaluate() {}
};

CLOSE_O2_NAMESPACE

#endif // ANIMATION_BASE_H
