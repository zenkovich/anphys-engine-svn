#include "scheduler.h"

#include "math\math.h"

OPEN_O2_NAMESPACE
	
DECLARE_SINGLETON(cScheduler);

bool cScheduler::Task::execute()
{
	if (mVariableRepeatDelay)
	{
		mDelay = ((IRetCallback<float>*)mCallback)->callWithRes();
	}
	else
	{
		mCallback->call();
		mDelay = mRepeatDelay;
	}

	return mDelay >= 0.0f;
}


cScheduler::cScheduler()
{
	mCurrentTasks = &mTasks[0];
	mNextTasks = &mTasks[1];

	const int initTasksCount = 25;
	for (int i = 0; i < initTasksCount; i++)
		mUnusedTasks.push_back(mnew Task);

	mLastTaskId = 0;
}

cScheduler::~cScheduler()
{
	FOREACH(TaskVec, mTasks[0], it)
		safe_release(*it);

	FOREACH(TaskVec, mTasks[1], it)
		safe_release(*it);

	FOREACH(TaskVec, mUnusedTasks, it)
		safe_release(*it);
}

void cScheduler::processBeforeFrame( float dt )
{
	processCurrentTasks(dt, ES_BEFORE_FRAME);
}

void cScheduler::processAfterFrame( float dt )
{
	processCurrentTasks(dt, ES_AFTER_FRAME);
	tswap(mCurrentTasks, mNextTasks);
	mNextTasks->clear();
}

void cScheduler::processCurrentTasks(float dt, ExecStage stage)
{
	FOREACH(TaskVec, *mCurrentTasks, task)
	{
		if ((*task)->mExecStage == stage)
		{
			(*task)->mDelay -= dt;

			if ((*task)->mDelay < 0)
			{
				if((*task)->execute())
				{
					mNextTasks->push_back(*task);
				}
				else
				{
					mUnusedTasks.push_back(*task);
				}
			}
			else
			{
				mNextTasks->push_back(*task);
			}
		}
	}
}

int cScheduler::addTask( sharedPtr(ICallback) callback, float execDelay /*= 0.0f*/, ExecStage stage /*= ES_AFTER_FRAME*/ )
{
	TaskVec* tasks = stage == ES_AFTER_FRAME ? mCurrentTasks:mNextTasks;
	sharedPtr(Task) newTask = getTask(callback, execDelay, -1, stage, false);
	tasks->push_back(newTask);
	return newTask->mId;
}

int cScheduler::addRepeatTask( sharedPtr(ICallback) callback, float repeatDelay, float execDelay /*= 0.0f*/, 
	                            ExecStage stage /*= ES_AFTER_FRAME*/ )
{
	TaskVec* tasks = stage == ES_AFTER_FRAME ? mCurrentTasks:mNextTasks;
	sharedPtr(Task) newTask = getTask(callback, execDelay, repeatDelay, stage, false);
	tasks->push_back(newTask);
	return newTask->mId;
}

int cScheduler::addRepeatTask( sharedPtr(IRetCallback<float>) callback, float execDelay /*= 0.0f*/, 
	                            ExecStage stage /*= ES_AFTER_FRAME*/ )
{
	TaskVec* tasks = stage == ES_AFTER_FRAME ? mCurrentTasks:mNextTasks;
	sharedPtr(Task) newTask = getTask((ICallback*)callback, execDelay, -1, stage, true);
	tasks->push_back(newTask);
	return newTask->mId;
}

void cScheduler::clearAllTasks()
{
	FOREACH(TaskVec, *mCurrentTasks, it)
		mUnusedTasks.push_back(*it);

	FOREACH(TaskVec, *mNextTasks, it)
		mUnusedTasks.push_back(*it);

	mCurrentTasks->clear();
	mNextTasks->clear();
}

void cScheduler::removeTask( int id )
{
	FOREACH(TaskVec, *mCurrentTasks, it)
	{
		if ((*it)->mId == id) 
		{
			mCurrentTasks->erase(it);
			mUnusedTasks.push_back(*it);
			break;
		}
	}
	
	FOREACH(TaskVec, *mNextTasks, it)
	{
		if ((*it)->mId == id) 
		{
			mNextTasks->erase(it);
			mUnusedTasks.push_back(*it);
			break;
		}
	}
}

sharedPtr(cScheduler::Task) cScheduler::getTask( sharedPtr(ICallback) callback, float delay, float repeatDelay, ExecStage stage, 
	                                   bool variableRepeatDelay )
{
	sharedPtr(Task) res;

	if (mUnusedTasks.size() > 0)
	{
		res = mUnusedTasks.back();
		mUnusedTasks.pop_back();
	}
	else
		res = mnew Task;

	res->mCallback = callback;
	res->mDelay = delay;
	res->mRepeatDelay = repeatDelay;
	res->mExecStage = stage;
	res->mVariableRepeatDelay = variableRepeatDelay;
	res->mId = mLastTaskId++;

	if (mLastTaskId == 9999999)
		mLastTaskId = 0;

	return res;
}

CLOSE_O2_NAMESPACE