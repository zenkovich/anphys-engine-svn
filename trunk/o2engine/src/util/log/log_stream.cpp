#include "log_stream.h"

#include <cstdarg>
#include <algorithm>
#include "util/mem_utils/alloc_operators.h"

OPEN_O2_NAMESPACE


LogStream::LogStream():
	mParentStream(NULL), mLevel(2)
{

}

LogStream::LogStream( const string& id ):
	mParentStream(NULL), mId(id), mLevel(2)
{
}

LogStream::~LogStream()
{
	if (mParentStream)
		mParentStream->unbindStream(this, false);
	unbindAllStreams();
}

void LogStream::setLevel( uint8 level )
{
	mLevel = level;

	for (LogSteamsVec::iterator it = mChildStreams.begin(); it != mChildStreams.end(); ++it)
		(*it)->setLevel(level);
}

uint8 LogStream::getLevel() const
{
	return mLevel;
}

const string& LogStream::getId() const
{
	return mId;
}

void LogStream::bindStream( LogStream* stream )
{
	stream->mParentStream = this;
	stream->mLevel = mLevel;
	mChildStreams.push_back(stream);
}

void LogStream::unbindStream( LogStream* stream, bool release /*= true*/ )
{
	LogSteamsVec::iterator fnd = std::find(mChildStreams.begin(), mChildStreams.end(), stream);
	if (fnd != mChildStreams.end())
		mChildStreams.erase(fnd);

	if (release)
		safe_release(stream);
}

void LogStream::unbindAllStreams()
{
	for (LogSteamsVec::iterator it = mChildStreams.begin(); it != mChildStreams.end(); ++it)
	{
		safe_release(*it);
	}

	mChildStreams.clear();
}

void LogStream::out( const char* format, ... )
{
	if (mLevel > 0)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024]; 
		vsprintf(buf, format, vlist);

		va_end(vlist);

		outStr(buf);
	}
}

void LogStream::hout( const char* format, ... )
{
	if (mLevel > 1)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024]; 
		vsprintf(buf, format, vlist);

		va_end(vlist);

		outStr(buf);
	}
}

void LogStream::error( const char* format, ... )
{
	if (mLevel > 0)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024]; 
		vsprintf(buf, format, vlist);

		va_end(vlist);

		outError(buf);
	}
}

void LogStream::warning( const char* format, ... )
{
	if (mLevel > 0)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024]; 
		vsprintf(buf, format, vlist);

		va_end(vlist);

		outWarning(buf);
	}
}

LogStream* LogStream::getParentStream() const
{
	return mParentStream;
}

void LogStream::outStr( const string& str )
{
	outStrEx(str);

	if (mParentStream)
	{
		if (mId == "")
			mParentStream->outStr(str);
		else			
			mParentStream->outStr(mId + ":" + str);
	}
}

void LogStream::outError( const string& str )
{
	outErrorEx(str);

	if (mParentStream)
	{
		if (mId == "")
			mParentStream->outError(str);
		else			
			mParentStream->outError(mId + ":" + str);
	}
}

void LogStream::outWarning( const string& str )
{
	outWarningEx(str);

	if (mParentStream)
	{
		if (mId == "")
			mParentStream->outWarning(str);
		else			
			mParentStream->outWarning(mId + ":" + str);
	}
}

CLOSE_O2_NAMESPACE