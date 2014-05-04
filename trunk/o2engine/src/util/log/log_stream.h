#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include "util/public_namespace.h"
#include "util/public_types.h"
OPEN_O2_NAMESPACE

/** Basic log stream. Contains interfaces of outing data, binding values, parent and child streams. */
class cLogStream
{
protected:
	/** Type of bind value. */
	enum BindValType { BV_INT = 0, BV_FLOAT, BV_BOOL, BV_CHAR_PTR, BV_STRING, BV_VEC2F };

	/** Binded value struct. Contains pointer to binded value, type, id. */
	struct BindValue
	{
		void*       mValuePtr;    /**< Pointer to binded value. */
		BindValType mType;        /**< Type of binded value. */
		char        mBuffer[256]; /**< Buffer for formatted data by type. */
		string      mId;          /**< Id of binded value. Using as prefix .*/

		BindValue(void* valuePtr, BindValType type, const string& id):
			mValuePtr(valuePtr), mType(type), mId(id) { mBuffer[0] = '\0'; }

		/** Returns formatted string data by value type. */
		const char* getStr();
	};
	typedef vector<BindValue> BindValVec;
	typedef vector<cLogStream*> LogSteamsVec;

	cLogStream*  mParentStream; /**< Parent stream. NULL if no parent. */

	string       mId;           /**< Name of log stream. */
	uint8        mLevel;        /**< Log level. */

	LogSteamsVec mChildStreams; /**< Child streams. */
	BindValVec   mBindedValues; /**< Binded values. */

public:
	cLogStream();
	cLogStream(const string& id);
	virtual ~cLogStream();

	/** Sets log level, for childes too. */
	void setLevel(uint8 level);

	/** Returns log level. */
	uint8 getLevel() const;

	/** Return name of stream. */
	const string& getId() const;

	/** Binding child stream. */
	void bindStream(cLogStream* stream);

	/** Unbinding child stream. Function destroying stream object. */
	void unbindStream(cLogStream* stream);

	/** Unbind and destroy all child streams. */
	void unbindAllStreams();

	/** Returns parent stream. Null if no parent. */
	cLogStream* getParentStream() const;

	/** Binding value. */
	void bindValue(void* valuePtr, BindValType type, const string& id);

	/** Unbind value. */
	void unbindvalue(void* valuePtr);

	/** Unbind all values. */
	void unbindAllValues();

	/** Out all binded values data. */
	void checkBindedValues();

	/** Out with low level log. */
	void out(const char* format, ...);

	/** Out with hight level log. */
	void hout(const char* format, ...);

	/** Out error message. */
	void error(const char* format, ...);

	/** Out warning message. */
	void warning(const char* format, ...);

protected:
	/** Out string to stream. */
	virtual void outStrEx(const string& str) {}

	/** Out error to stream. */
	virtual void outErrorEx(const string& srt) { outStrEx("ERROR:" + srt); }

	/** Out warning to stream. */
	virtual void outWarningEx(const string& srt) { outStrEx("WARNING:" + srt); }

	/** Out string to current stream and parent stream. */
	void outStr(const string& str);

	/** Out error to current stream and parent stream. */
	void outError(const string& str);

	/** Out warning to current stream and parent stream. */
	void outWarning(const string& str);
};

CLOSE_O2_NAMESPACE

#endif //LOG_STREAM_H