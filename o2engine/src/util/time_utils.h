#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include "public.h"
#include "singleton.h"

OPEN_O2_NAMESPACE

struct WideTime 
{
	int mYear;
	int mMonth;
	int mDay;
	int mHour;
	int mMinute;
	int mSecond;

	WideTime(int seconds = 0, int minutes = 0, int hours = 0, int days = 0, int months = 0, int years = 0)
		:mYear(years), mMonth(months), mDay(days), mHour(hours), mMinute(minutes), mSecond(seconds)
	{
	}

	bool operator==(WideTime& wt)
	{
		return mSecond == wt.mSecond && mMinute == wt.mMinute && mHour == wt.mHour && mDay == wt.mDay && mMonth == wt.mMonth &&
			   mYear == wt.mYear;
	}

	bool operator!=(WideTime& wt)
	{
		return !(*this == wt);
	}
};

class cTimeUtil: public cSingleton<cTimeUtil>
{
	friend class cApplicationBaseInterface;
	friend class cApplication;

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

	/** Update parameters. */
	void update(float dt);

public:
	/** Returns application working time, in seconds. */
	float getApplicationTime() const;

	/** Returns local time, in seconds. */
	float getLocalTime() const;

	/** Resetting local time to zero. */
	void resetLocalTime();

	/** Resetting local time to value. */
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