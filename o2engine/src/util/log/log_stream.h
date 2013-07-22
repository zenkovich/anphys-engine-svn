#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <vector>
#include <list>

#include "public.h"
OPEN_O2_NAMESPACE

class cLogStream
{
	enum BindValType { BV_INT = 0, BV_FLOAT, BV_BOOL, BV_CHAR_PTR, BV_STRING, BV_VEC2F };

	struct BindValue
	{
		void*       mValuePtr;
		BindValType mType;
		char        mBuffer[256];
		std::string mId;

		BindValue(void* valuePtr, BindValType type, const std::string& id):
			mValuePtr(valuePtr), mType(type), mId(id) { mBuffer[0] = '\0'; }

		const char* getStr();
	};
	typedef std::vector<BindValue> BindValVec;
	typedef std::vector<cLogStream*> LogSteamsVec;

	cLogStream*  mParentStream;

	std::string  mId;
	uint8        mLevel;

	LogSteamsVec mChildStreams;
	BindValVec   mBindedValues;

public:
	cLogStream();
	cLogStream(const std::string& id);
	virtual ~cLogStream();

	void setLevel(uint8 level);
	uint8 getLevel() const;

	const std::string& getId() const;

	void bindStream(cLogStream* stream);
	void unbindStream(cLogStream* stream);
	void unbindAllStreams();

	void bindValue(void* valuePtr, BindValType type, const std::string& id);
	void unbindvalue(void* valuePtr);
	void unbindAllValues();

	void checkBindedValues();

	void out(const char* format, ...);
	void hout(const char* format, ...);

protected:
	virtual void outStr(const std::string& str) {}
};

CLOSE_O2_NAMESPACE

#endif //LOG_STREAM_H