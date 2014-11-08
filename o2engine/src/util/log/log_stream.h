#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include "util/public_namespace.h"
#include "util/public_types.h"


OPEN_O2_NAMESPACE

/** Basic log stream. Contains interfaces of outing data, parent and child streams. */
class cLogStream
{
protected:
	typedef vector< cLogStream* > LogSteamsVec;

	cLogStream*  mParentStream; /**< Parent stream. NULL if no parent. */

	string       mId;           /**< Name of log stream. */
	uint8        mLevel;        /**< Log level. */

	LogSteamsVec mChildStreams; /**< Child streams. */

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
	void unbindStream(cLogStream* stream, bool release = true);

	/** Unbind and destroy all child streams. */
	void unbindAllStreams();

	/** Returns parent stream. Null if no parent. */
	cLogStream* getParentStream() const;

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