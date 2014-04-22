#include "file_log_stream.h"

#include "util/file_system/file.h"

OPEN_O2_NAMESPACE

cFileLogStream::cFileLogStream( uint8 level, const string& fileName ):
	cLogStream(), mOutFile(NULL)
{
	setLevel(level);

	if (level > 0)
		openFile(fileName);
}

cFileLogStream::cFileLogStream( const string& id, uint8 level, const string& fileName ):
	cLogStream(id), mOutFile(NULL)
{
	setLevel(level);

	if (level > 0)
		openFile(fileName);
}

cFileLogStream::~cFileLogStream()
{
	safe_release(mOutFile);
}

void cFileLogStream::outStrEx( const string& str )
{
	if (mOutFile)
	{
		mOutFile->writeData((void*)(str + '\n').c_str(), str.length() + 1);
	}
}

void cFileLogStream::openFile( const string& fileName )
{
	mOutFile = mnew cOutFile(fileName);
}

void cFileLogStream::outErrorEx( const string& srt )
{
	if (mOutFile)
	{
		mOutFile->writeData((void*)("ERROR:" + srt + '\n').c_str(), srt.length() + 7);
	}
}

void cFileLogStream::outWarningEx( const string& srt )
{
	if (mOutFile)
	{
		mOutFile->writeData((void*)("WARNING:" + srt + '\n').c_str(), srt.length() + 9);
	}
}

CLOSE_O2_NAMESPACE