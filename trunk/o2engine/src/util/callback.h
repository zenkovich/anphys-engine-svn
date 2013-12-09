#ifndef CALLBACK_H
#define CALLBACK_H

#include <vector>
#include <cstdarg>
#include <algorithm>

#include "public.h"

OPEN_O2_NAMESPACE
	
struct Dummy
{
	Dummy() {}
	~Dummy() {}
};


class ICallback
{
public:
	virtual ~ICallback() {}
	virtual void call() = 0;
	virtual ICallback* clone() const = 0;
};


class cCallbackChain:public ICallback
{
public:
	typedef std::vector<ICallback*> CallbacksVec;

protected:
	CallbacksVec mCallbacks;

public:
	cCallbackChain() {}

	cCallbackChain(int count, ...)
	{
		va_list vlist;
		va_start(vlist, count);

		for (int i = 0; i < count; i++) 
			mCallbacks.push_back(va_arg(vlist, ICallback*));

		va_end(vlist);
	}

	cCallbackChain(const cCallbackChain& callbackChain)
	{
		for (CallbacksVec::const_iterator it = callbackChain.mCallbacks.cbegin(); it != callbackChain.mCallbacks.cend(); 
			 ++it)
		{
			mCallbacks.push_back((*it)->clone());
		}
	}

	~cCallbackChain()
	{
		for (CallbacksVec::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
			safe_release(*it);
	}

	void add(ICallback* callback)
	{
		mCallbacks.push_back(callback);
	}

	void remove(ICallback* callback) 
	{
		CallbacksVec::iterator fnd = std::find(mCallbacks.begin(), mCallbacks.end(), callback);
		if (fnd != mCallbacks.end())
			mCallbacks.erase(fnd);

		safe_release(callback);
	}

	void call()
	{
		for (CallbacksVec::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
			(*it)->call();
	}

	ICallback* clone() const
	{
		return mnew cCallbackChain(*this);
	}
};

inline ICallback* callbackChain(int count, ...) 
{
	cCallbackChain* res = mnew cCallbackChain();
	va_list vlist;
	va_start(vlist, count);

	for (int i = 0; i < count; i++) 
		res->add(va_arg(vlist, ICallback*));

	va_end(vlist);

	return res;
}


template<typename T = Dummy>
class cCallback:public ICallback
{
	T* mObject;
	void (T::*mObjectFunction)();
	void (*mFunction)();

public:
	cCallback(T* object, void (T::*function)()):
	  mObject(object), mObjectFunction(function) {}

	cCallback(void (*function)()):
	  mObject(NULL), mObjectFunction(NULL), mFunction(function) {}

	cCallback(const cCallback& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
	}

	void call()
	{
		if (mObject && mObjectFunction) (mObject->*mObjectFunction)();
		else if (mFunction) (*mFunction)();
	}

	ICallback* clone() const
	{
		return mnew cCallback<T>(*this);
	}
};

template<typename T>
ICallback* callback(T* object, void (T::*function)()) { return mnew cCallback<T>(object, function); }

inline ICallback* callback(void (*function)()) { return mnew cCallback<Dummy>(function); }


template<typename ArgT, typename T = Dummy>
class cCallback1Param:public ICallback
{
	ArgT mArg;
	T* mObject;
	void (T::*mObjectFunction)(ArgT);
	void (*mFunction)(ArgT);

public:
	cCallback1Param(T* object, void (T::*function)(ArgT), const ArgT& arg):
	  mObject(object), mObjectFunction(function) { mArg = arg; }

	cCallback1Param(void (*function)(ArgT), const ArgT& arg):
	  mObject(NULL), mObjectFunction(NULL), mFunction(function) { mArg = arg; }

	cCallback1Param(const cCallback1Param<ArgT, T>& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
		mArg = callback.mArg;
	}

	void call()
	{
		if (mObject && mObjectFunction) (mObject->*mObjectFunction)(mArg);
		else if (mFunction) (*mFunction)(mArg);
	}
	
	ICallback* clone() const 
	{
		return mnew cCallback1Param<ArgT, T>(*this);
	}
};

template<typename ArgT, typename T>
ICallback* callback(T* object, void (T::*function)(ArgT), const ArgT& arg)
{ 
	return mnew cCallback1Param<T>(object, function, arg);
}

template<typename ArgT>
ICallback* callback(void (*function)(ArgT), const ArgT& arg) 
{
	return mnew cCallback1Param(function, arg);
}


template<typename ArgT, typename ArgT2, typename T = Dummy>
class cCallback2Param:public ICallback
{
	ArgT  mArg;
	ArgT2 mArg2;
	T*    mObject;
	void (T::*mObjectFunction)(ArgT, ArgT2);
	void (*mFunction)(ArgT, ArgT2);

public:
	cCallback2Param(T* object, void (T::*function)(ArgT, ArgT2), const ArgT& arg1, const ArgT2& arg2 ):
		mObject(object), mObjectFunction(function), mArg(arg1), mArg2(arg2) {}

	cCallback2Param(void (*function)(ArgT, ArgT2), const ArgT& arg1, const ArgT2& arg2):
		mObject(NULL), mObjectFunction(NULL), mFunction(function), mArg(arg1), mArg2(arg2) {}
	
	cCallback2Param(const cCallback2Param<ArgT, ArgT2, T>& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
		mArg = callback.mArg;
		mArg2 = callback.mArg2;
	}

	void call()
	{
		if (mObject && mObjectFunction) (mObject->*mObjectFunction)(mArg, mArg2);
		else if (mFunction) (*mFunction)(mArg, mArg2);
	}
	
	ICallback* clone() const 
	{
		return mnew cCallback2Param<ArgT, ArgT2, T>(*this);
	}
};

template<typename ArgT, typename ArgT2, typename T>
ICallback* callback(T* object, void (T::*function)(ArgT, ArgT2), const ArgT& arg, const ArgT2& arg2)
{ 
	return mnew cCallback2Param<T>(object, function, arg, arg2);
}

template<typename ArgT, typename ArgT2>
ICallback* callback(void (*function)(ArgT, ArgT2), const ArgT& arg, const ArgT2& arg2) 
{
	return mnew cCallback2Param(function, arg, arg2);
}

CLOSE_O2_NAMESPACE

#endif //CALLBACK_H