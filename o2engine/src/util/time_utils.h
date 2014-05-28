#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include "public.h"
#include "singleton.h"

OPEN_O2_NAMESPACE

class cTimeUtil: public cSingleton<cTimeUtil>, public virtual ShareObj
{
	friend class cApplicationBaseInterface;
	friend class cApplication;
	friend class shared<cTimeUtil>;

	float   mApplicationTime;     /** Application working time, seconds. */
	float   mLocalTime;           /** Local timer working, seconds. Controlling by user. */
	uint64  mCurrentFrame;        /** Current frame index. */
	float   mDeltaTime;           /** Frame delta time. */
	float   mFPS;                 /** Average frames per second. */
	 
	float   mFPSSum;              /** Summary of fps. */
	float   mFramesSum;           /** Frames summary. */
	float   mLastFPSCheckingTime; /** Last average fps checking time. */

	/** ctor. */
	cTimeUtil();

	/** dtor. */
	~cTimeUtil();

	/** Update parametres. */
	void update(float dt);

public:
	/** Returns application working time, in seconds. */
	float getApplicationTime() const;

	/** Returns local time, in seconds. */
	float getLocalTime() const;

	/** Resetting local time to zero. */
	void resetLocalTime();

	/** Resetting local time to calue. */
	void setLocalTime(float time);

	/** Returns current frame index. */
	int getCurrentFrame() const;

	/** Returns frames delta time. */
	float getDeltaTime() const;

	/** Returns average frames per second for 0.3 seconds. */
	float getFPS() const;
};

#define timeUtils() cTimeUtil::instancePtr()

CLOSE_O2_NAMESPACE

#endif //TIME_UTILS_H