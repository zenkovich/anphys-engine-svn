#include "file_log_stream.h"

#include "util/file_system/file.h"

OPEN_O2_NAMESPACE

FileLogStream::FileLogStream( uint8 level, const string& fileName ):
	LogStream(), mOutFile(NULL)
{
	setLevel(level);

	if (level > 0)
		openFile(fileName);
}

FileLogStream::FileLogStream( const string& id, uint8 level, const string& fileName ):
	LogStream(id), mOutFile(NULL)
{
	setLevel(level);

	if (level > 0)
		openFile(fileName);
}

FileLogStream::~FileLogStream()
{
	safe_release(mOutFile);
}

void FileLogStream::outStrEx( const string& str )
{
	if (mOutFile)
	{
		mOutFile->writeData((void*)(str + '\n').c_str(), str.length() + 1);
	}
}

void FileLogStream::openFile( const string& fileName )
{
	mOutFile = mnew OutFile(fileName);
}

void FileLogStream::outErrorEx( const string& srt )
{
	if (mOutFile)
	{
		mOutFile->writeData((void*)("ERROR:" + srt + '\n').c_str(), srt.length() + 7);
	}
}

void FileLogStream::outWarningEx( const string& srt )
{
	if (mOutFile)
	{
		mOutFile->writeData((void*)("WARNING:" + srt + '\n').c_str(), srt.length() + 9);
	}
}

CLOSE_O2_NAMESPACE