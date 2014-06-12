#ifndef THREAD_H
#define THREAD_H

#include "public.h"

#include "pthreads/pthread.h"
#include "util/smart_ptrs.h"
#include "util/callback.h"

OPEN_O2_NAMESPACE

//class ICallback;

/** Thread starter class. Uses POSIX. */
class cThread
{
public:
	enum ThreadPriority { TP_NORMAL = 0, TP_LOW, TP_HIGHT };

protected:
	pthread_t         mThreadId;   /** Thread id. */
	shared<ICallback> mThreadFunc; /** Function starting in thread. */
	bool              mStarted;    /** True, when started. */

	/** copy ctor. */
	cThread(const cThread& thread) {}

	/** copy operator. */
	cThread& operator=(const cThread& thread) { return *this; }

	/** Thead function. Here calls mThreadFunc. */
	static void* threadFunc(void* arg);

public:
	/** ctor. */
	cThread();

	/** dtor. */
	~cThread();

	/** Starts function with priority. */
	int start(shared<ICallback> threadCallback, ThreadPriority threadPriority = TP_NORMAL);

	/** Join. */
	int join();

	/** Cancelling thread. */
	int cancel();
};

CLOSE_O2_NAMESPACE

#endif //THREAD_H