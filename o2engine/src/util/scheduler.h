#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "public.h"

#include "singleton.h"
#include "callback.h"
#include "smart_ptrs.h"

OPEN_O2_NAMESPACE

/** Scheduler class. Processing tasks at begin and end frame. */
class cScheduler: public cSingleton<cScheduler>
{
	friend class cApplication;
	friend class cApplicationBaseInterface;

public:
	enum ExecStage { ES_BEFORE_FRAME = 0, ES_AFTER_FRAME };

protected:
	/** Task container struct. */
	struct Task
	{
		float          mDelay;               /** Delay to next execution. */
		float          mRepeatDelay;         /** Repeat delay. If no repeat == -1. */
		ExecStage      mExecStage;           /** Execution stage. */
		sharedPtr(ICallback) mCallback;            /** Execution callback. */
		bool           mVariableRepeatDelay; /** True, when repeat delay will be get from callback result. */
		int            mId;                  /** Task id. */
		
		/** Executes the callback and returning true, when task must be repeated. */
		bool execute();
	};
	typedef vector<sharedPtr(Task)> TaskVec;

	TaskVec  mTasks[2];     /** Tasks vectors. One for current tasks, other for next tasks. */
	TaskVec* mCurrentTasks; /** Current tasks vector ptr. */
	TaskVec* mNextTasks;    /** Next tasks vector ptr. */
	TaskVec  mUnusedTasks;  /** Unused tasks vector. When needs some task object, first will take from here. */
	int      mLastTaskId;   /** Last task id. */
	
	/** ctor. */
	cScheduler();

	/** dtor. */
	~cScheduler();

public:	
	/** Adding once time execution callback. Return id of task. */
	int addTask(sharedPtr(ICallback) callback, float execDelay = 0.0f, ExecStage stage = ES_AFTER_FRAME);

	/** Adding repeating callback, repeats with repeatDelay seconds and never stops. Returns task id. */
	int addRepeatTask(sharedPtr(ICallback) callback, float repeatDelay, float execDelay = 0.0f, ExecStage stage = ES_AFTER_FRAME);

	/** Adding repeating callback, repeat delay gets from callback result. Returns task id. */
	int addRepeatTask(sharedPtr(IRetCallback<float>) callback, float execDelay = 0.0f, ExecStage stage = ES_AFTER_FRAME);

	/** Removes task by id, task will never executed. */
	void removeTask(int id);

	/** Removes all tasks. */
	void clearAllTasks();

protected:
	/** Processing tasks before frame. */
	void processBeforeFrame(float dt);

	/** Processing tasts after frame. */
	void processAfterFrame(float dt);

	/** Processing mCurrentTasks with specified dt and stage. */
	void processCurrentTasks(float dt, ExecStage stage);

	/** Returns task sample filled with specified parametres. */
	sharedPtr(Task) getTask(sharedPtr(ICallback) callback, float delay, float repeatDelay, ExecStage stage, bool variableRepeatDelay);
};

#define scheduler() cScheduler::instancePtr()

CLOSE_O2_NAMESPACE

#endif //SCHEDULER_H