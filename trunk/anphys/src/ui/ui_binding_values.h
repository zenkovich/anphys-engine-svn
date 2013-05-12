#ifndef UI_BINDING_VALUES_H
#define UI_BINDING_VALUES_H

#include <vector>
#include <string>
#include <algorithm>
#include "util\memory\mem_utils.h"
#include "util\other\string_utils.h"

struct uiBindingValues
{
	struct BindValuePrototype
	{
		enum TypeId { t_string = 0, t_float, t_integer, t_bool };

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

		BindValue():mValuePtr(NULL) {}
		BindValue(TType* valuePtr):mValuePtr(valuePtr) { /*mLastValue = *mValuePtr;*/ }
		
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
			bool res = *mValuePtr != mLastValue;
			mLastValue = *mValuePtr;
			return res;
		}
		
		bool isBindingValue(const void* valuePtr) const
		{
			return mValuePtr == valuePtr;
		}
	};

	typedef std::vector<BindValuePrototype*> BindValuesList;

	BindValuesList mBindValues;

//functions
	uiBindingValues() {}
	~uiBindingValues() { clearBindingValues(); }

	template<typename T>
	void bindValue(T* valuePtr)
	{
		BindValuePrototype* newBindValue = new BindValue<T>(valuePtr);
		mBindValues.push_back(newBindValue);
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
	static void conv(const float& in, int& out)
	{
		out = (int)in;
	}
	static void conv(const float& in, bool& out)
	{
		out = in != 0.0f;
	}
};

#endif //UI_BINDING_VALUES_H