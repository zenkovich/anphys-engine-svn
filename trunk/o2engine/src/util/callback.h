#ifndef CALLBACK_H
#define CALLBACK_H

#include <cstdarg>

#include "public.h"
#include "smart_ptrs.h"

OPEN_O2_NAMESPACE
	
/** Stupid simple class. */
struct Dummy
{
	Dummy() {}
	~Dummy() {}
};


/** Callback interface. */
class ICallback
{
public:
	virtual ~ICallback() {}
	virtual void call() = 0;
	virtual void call(void* param, ...) { call(); }
	virtual ICallback* clone() const = 0;
};

/** Callback with return value interface. */
template<typename RetType>
class IRetCallback: public ICallback
{
public:
	virtual ~IRetCallback() {}
	virtual RetType callWithRes() = 0;
	virtual RetType callWithRes(void* param, ...) { return callWithRes(); };
	virtual void call() { callWithRes(); }
};


/************************************************************************/
/** Callback without parametres and with return value. */
/************************************************************************/
template<typename RetType, typename ClassType = Dummy>
class cRetCallback:public IRetCallback<RetType>
{
	shared<ClassType> mObject;
	RetType (ClassType::*mObjectFunction)();
	RetType (*mFunction)();

public:
	cRetCallback(shared<ClassType> object, RetType (ClassType::*function)()):
	  mObject(object), mObjectFunction(function) {}

	cRetCallback(RetType (*function)()):
	  mObject(NULL), mObjectFunction(NULL), mFunction(function) {}

	cRetCallback(const cRetCallback& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
	}

	RetType callWithRes()
	{
		if (mObject && mObjectFunction) 
			return (mObject->*mObjectFunction)();
		else 
			if (mFunction) 
				return (*mFunction)();
	}

	ICallback* clone() const
	{
		return mnew cRetCallback<RetType, ClassType>(*this);
	}
};

/** Fast callback creation function. */
template<typename RetType, typename ClassType>
IRetCallback<RetType>* callback(shared<ClassType> object, RetType (ClassType::*function)()) 
{
	return mnew cRetCallback<RetType, ClassType>(object, function);
}

/** Fast callback creation function. */
template<typename RetType>
inline IRetCallback<RetType>* callback(RetType (*function)()) 
{
	return mnew cRetCallback<RetType, Dummy>(function); 
}


/************************************************************************/
/** Callbacks chain. */
/************************************************************************/
class cCallbackChain:public ICallback
{
public:
	typedef vector<shared<ICallback>> CallbacksVec;

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
		FOREACH_CONST(CallbacksVec, callbackChain.mCallbacks, it)
		{
			mCallbacks.push_back((*it)->clone());
		}
	}

	~cCallbackChain()
	{
		removeAll();
	}

	void add(shared<ICallback> callback)
	{
		mCallbacks.push_back(callback);
	}

	void remove(shared<ICallback> callback) 
	{
		CallbacksVec::iterator fnd = std::find(mCallbacks.begin(), mCallbacks.end(), callback);
		if (fnd != mCallbacks.end())
			mCallbacks.erase(fnd);

		safe_release(callback);
	}

	void removeAll()
	{
		for (CallbacksVec::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
			safe_release(*it);

		mCallbacks.clear();
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


/** Fast callback chain creation function. */
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


/************************************************************************/
/** Callback without parametres. */
/************************************************************************/
template<typename ClassType = Dummy>
class cCallback:public ICallback
{
	shared<ClassType> mObject;
	void (ClassType::*mObjectFunction)();
	void (*mFunction)();

public:
	cCallback(shared<ClassType> object, void (ClassType::*function)()):
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
		return mnew cCallback<ClassType>(*this);
	}
};

/** Fast callback creation function. */
template<typename ClassType>
ICallback* callback(shared<ClassType> object, void (ClassType::*function)()) { return mnew cCallback<ClassType>(object, function); }

/** Fast callback creation function. */
inline ICallback* callback(void (*function)()) { return mnew cCallback<Dummy>(function); }


/************************************************************************/
/** Callback with 1 parameter. */
/************************************************************************/
template<typename ArgT, typename ClassType = Dummy>
class cCallback1Param:public ICallback
{
	ArgT       mArg;
	shared<ClassType> mObject;
	void (ClassType::*mObjectFunction)(ArgT);
	void (*mFunction)(ArgT);

public:
	cCallback1Param(shared<ClassType> object, void (ClassType::*function)(ArgT), const ArgT& arg):
	  mObject(object), mObjectFunction(function) { mArg = arg; }

	cCallback1Param(void (*function)(ArgT), const ArgT& arg):
	  mObject(NULL), mObjectFunction(NULL), mFunction(function) { mArg = arg; }

	cCallback1Param(const cCallback1Param<ArgT, ClassType>& callback)
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

	void call(void* param, ...)
	{
		mArg = *((ArgT*)param);
		call();
	}
	
	ICallback* clone() const 
	{
		return mnew cCallback1Param<ArgT, ClassType>(*this);
	}
};

/** Fast callback1 creation function. */
template<typename ArgT, typename ClassType>
ICallback* callback(shared<ClassType> object, void (ClassType::*function)(ArgT), const ArgT& arg)
{ 
	return mnew cCallback1Param<ArgT, ClassType>(object, function, arg);
}

/** Fast callback1 creation function. */
template<typename ArgT>
ICallback* callback(void (*function)(ArgT), const ArgT& arg) 
{
	return mnew cCallback1Param<ArgT>(function, arg);
}


/************************************************************************/
/** Callback with 2 parameters. */
/************************************************************************/
template<typename ArgT, typename ArgT2, typename ClassType = Dummy>
class cCallback2Param:public ICallback
{
	ArgT       mArg;
	ArgT2      mArg2;
	shared<ClassType> mObject;
	void (ClassType::*mObjectFunction)(ArgT, ArgT2);
	void (*mFunction)(ArgT, ArgT2);

public:
	cCallback2Param(shared<ClassType> object, void (ClassType::*function)(ArgT, ArgT2), const ArgT& arg1, const ArgT2& arg2 ):
		mObject(object), mObjectFunction(function), mArg(arg1), mArg2(arg2) {}

	cCallback2Param(void (*function)(ArgT, ArgT2), const ArgT& arg1, const ArgT2& arg2):
		mObject(NULL), mObjectFunction(NULL), mFunction(function), mArg(arg1), mArg2(arg2) {}
	
	cCallback2Param(const cCallback2Param<ArgT, ArgT2, ClassType>& callback)
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

	void call(void* param, ...)
	{
		va_list vlist;
		va_start(vlist, param);
		
		mArg = *((ArgT*)param);
		mArg = va_arg(vlist, ArgT2);

		va_end(vlist);

		call();
	}
	
	ICallback* clone() const 
	{
		return mnew cCallback2Param<ArgT, ArgT2, ClassType>(*this);
	}
};

/** Fast callback2 creation function. */
template<typename ArgT, typename ArgT2, typename ClassType>
ICallback* callback(shared<ClassType> object, void (ClassType::*function)(ArgT, ArgT2), const ArgT& arg, const ArgT2& arg2)
{ 
	return mnew cCallback2Param<ClassType>(object, function, arg, arg2);
}

/** Fast callback2 creation function. */
template<typename ArgT, typename ArgT2>
ICallback* callback(void (*function)(ArgT, ArgT2), const ArgT& arg, const ArgT2& arg2) 
{
	return mnew cCallback2Param(function, arg, arg2);
}

CLOSE_O2_NAMESPACE

#endif //CALLBACK_H