#ifndef UI_BINDING_VALUES_H
#define UI_BINDING_VALUES_H

#include <vector>
#include <string>
#include <algorithm>

#include "util\memory\mem_utils.h"
#include "util\other\string_utils.h"
#include "util\other\callback.h"

struct uiBindingValues
{
	struct BindValuePrototype
	{
		enum TypeId { t_string = 0, t_float, t_integer, t_bool };

		BindValuePrototype() {}
		virtual ~BindValuePrototype() {}

		virtual void setValue(const void* valuePtr, TypeId type) {}
		virtual void getValue(const void* valuePtr, TypeId type) const {}
		virtual bool checkValue() { return false; }
		virtual bool isBindingValue(const void* valuePtr) const { return false; }	
	};

	template<typename TType>
	struct BindValue:public BindValuePrototype
	{
		TType* mValuePtr;
		TType  mLastValue;

		bool   mMinLimitation;
		TType  mMinValue;
		bool   mMaxLimitation;
		TType  mMaxValue;

		cCallbackInterface* mChangeValueCallback;

		BindValue():mValuePtr(NULL), mMinLimitation(false), mMaxLimitation(false), mChangeValueCallback(NULL) {}
		BindValue(TType* valuePtr):mValuePtr(valuePtr), mMinLimitation(false), mMaxLimitation(false), 
			mChangeValueCallback(NULL) { /*mLastValue = *mValuePtr;*/ }
		~BindValue()
		{
			safe_release(mChangeValueCallback);
		}

		void setChangeValueCallback(cCallbackInterface* callback)
		{
			safe_release(mChangeValueCallback);
			mChangeValueCallback = callback;
		}
		
		void setValue(const void* valuePtr, TypeId type)
		{			
			if (type == t_string)
				conv(*(std::string*)valuePtr, *mValuePtr);
			else if (type == t_float)
				conv(*(float*)valuePtr, *mValuePtr);
			else if (type == t_integer)
				conv(*(int*)valuePtr, *mValuePtr);
			else if (type == t_bool)
				conv(*(bool*)valuePtr, *mValuePtr);

			mLastValue = *mValuePtr;

			if (mChangeValueCallback)
				mChangeValueCallback->call();
		}
		
		void getValue(const void* valuePtr, TypeId type) const
		{
			if (type == t_string)
				conv(*mValuePtr, *(std::string*)valuePtr);
			else if (type == t_float)
				conv(*mValuePtr, *(float*)valuePtr);
			else if (type == t_integer)
				conv(*mValuePtr, *(int*)valuePtr);
			else if (type == t_bool)
				conv(*mValuePtr, *(bool*)valuePtr);
		}

		bool checkValue()
		{
			if (mMinLimitation && comp(mMinValue, *mValuePtr))
				*mValuePtr = mMinValue;

			if (mMaxLimitation && comp(*mValuePtr, mMaxValue))
				*mValuePtr = mMaxValue;

			bool res = *mValuePtr != mLastValue;
			mLastValue = *mValuePtr;

			if (res && mChangeValueCallback)
				mChangeValueCallback->call();

			return res;
		}
		
		bool isBindingValue(const void* valuePtr) const
		{
			return mValuePtr == valuePtr;
		}

		void setLimitations(const TType& minvalue, const TType& maxvalue)
		{
			mMinLimitation = mMaxLimitation = true;
			mMinValue = minvalue;
			mMaxValue = maxvalue;
		}

		void setMinLimitation(const TType& minValue)
		{
			mMinLimitation = true;
			mMinValue = minValue;
		}

		void setMaxLimitation(const TType& maxValue)
		{
			mMaxLimitation = true;
			mMaxValue = maxValue;
		}
	};

	typedef std::vector<BindValuePrototype*> BindValuesList;

	BindValuesList mBindValues;

//functions
	uiBindingValues() {}
	~uiBindingValues() { clearBindingValues(); }

	template<typename T>
	BindValue<T>* bindValue(T* valuePtr)
	{
		BindValue<T>* newBindValue = new BindValue<T>(valuePtr);
		mBindValues.push_back(newBindValue);
		derivedUpdate(0.0f);
		return newBindValue;
	}

	template<typename T>
	void unbindValue(T* valuePtr)
	{
		for (BindValuesList::iterator it = mBindValues.begin(); it != mBindValues.end(); ++it)
		{
			if ((*it)->isBindingValue(valuePtr))
			{
				safe_release(*it);
				mBindValues.erase(it);
				return;
			}
		}
	}

	void clearBindingValues()
	{
		for (BindValuesList::iterator it = mBindValues.begin(); it != mBindValues.end(); ++it)
			safe_release(*it);
		mBindValues.clear();
	}

	template<typename T>
	BindValue<T>* getBindedValue(T* valuePtr)
	{
		for (BindValuesList::iterator it = mBindValues.begin(); it != mBindValues.end(); ++it)
		{
			if ((*it)->isBindingValue(valuePtr))
			{
				return static_cast<BindValue<T>*>(*it);
			}
		}
	}

	virtual void derivedUpdate(float dt) {}

protected:
	void setBindingValue(const void* valuePtr, BindValuePrototype::TypeId type)
	{
		for (BindValuesList::iterator it = mBindValues.begin(); it != mBindValues.end(); ++it)
		{
			(*it)->setValue(valuePtr, type);
		}
	}

//convertions
	template<typename T>
	static void conv(const T& in, T& out) { out = in; }

	static void conv(const std::string& in, float& out)
	{
		out = StrToFloat(in.c_str());
	}
	static void conv(const std::string& in, int& out)
	{
		out = StrToInt(in.c_str());
	}
	static void conv(const std::string& in, bool& out)
	{
		out = !(in == "false" || in == "0" || in == "");
	}
	static void conv(const int& in, float& out)
	{
		out = (float)in;
	}
	static void conv(const bool& in, float& out)
	{
		out = (float)in;
	}
	static void conv(const float& in, std::string& out)
	{
		out = toStr(in);
	}
	static void conv(const int& in, std::string& out)
	{
		out = toStr(in);
	}
	static void conv(const float& in, int& out)
	{
		out = (int)in;
	}
	static void conv(const float& in, bool& out)
	{
		out = in != 0.0f;
	}
	static void conv(const int& in, bool& out)
	{
		out = in != 0;
	}

	template<typename T>
	static bool comp(const T& a, const T& b) //a > b
	{
		return a > b;
	}

	bool comp(const std::string& a, const std::string& b)
	{
		return false;
	}
};

#endif //UI_BINDING_VALUES_H