#include "log_stream.h"

#include <cstdarg>
#include <algorithm>
#include "util/mem_utils/alloc_operators.h"

OPEN_O2_NAMESPACE


cLogStream::cLogStream():
	mParentStream(NULL), mLevel(2)
{

}

cLogStream::cLogStream( const string& id ):
	mParentStream(NULL), mId(id), mLevel(2)
{
}

cLogStream::~cLogStream()
{
	if (mParentStream)
		mParentStream->unbindStream(this, false);
	unbindAllStreams();
}

void cLogStream::setLevel( uint8 level )
{
	mLevel = level;

	for (LogSteamsVec::iterator it = mChildStreams.begin(); it != mChildStreams.end(); ++it)
		(*it)->setLevel(level);
}

uint8 cLogStream::getLevel() const
{
	return mLevel;
}

const string& cLogStream::getId() const
{
	return mId;
}

void cLogStream::bindStream( cLogStream* stream )
{
	stream->mParentStream = this;
	stream->mLevel = mLevel;
	mChildStreams.push_back(stream);
}

void cLogStream::unbindStream( cLogStream* stream, bool release /*= true*/ )
{
	LogSteamsVec::iterator fnd = std::find(mChildStreams.begin(), mChildStreams.end(), stream);
	if (fnd != mChildStreams.end())
		mChildStreams.erase(fnd);

	if (release)
		safe_release(stream);
}

void cLogStream::unbindAllStreams()
{
	for (LogSteamsVec::iterator it = mChildStreams.begin(); it != mChildStreams.end(); ++it)
	{
		safe_release(*it);
	}

	mChildStreams.clear();
}

void cLogStream::out( const char* format, ... )
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

void cLogStream::hout( const char* format, ... )
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

void cLogStream::error( const char* format, ... )
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

void cLogStream::warning( const char* format, ... )
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

cLogStream* cLogStream::getParentStream() const
{
	return mParentStream;
}

void cLogStream::outStr( const string& str )
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

void cLogStream::outError( const string& str )
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

void cLogStream::outWarning( const string& str )
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