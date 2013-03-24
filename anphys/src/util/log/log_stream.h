#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <string>
#include <cstdarg>
#include "engine/engine_options.h"

/** ����� ���-�������. ������ � ���� ��������� �� ������� ����� � ����, ������������ �� ������� �����. */
struct cLogStream
{
	cLogStream*  mMainStream;     /**< ����������� ������� �����. ���, ��� ����� �������� � ���� �����, ����� ��������� � � �������. */
	bool         mUsesMainStream; /**< ������������ �� ������� �����. */
	std::string  mStreamName;     /**< ��� ������. */
	unsigned int mLogLevel;

//functions
	cLogStream(): mMainStream(NULL), mUsesMainStream(false), mLogLevel(STD_LOG_LEVEL) {}
	cLogStream(cLogStream* mainStream): mMainStream(mainStream), mUsesMainStream(true), mLogLevel(STD_LOG_LEVEL) {}

	/** �������-�����. */
	virtual void output(const std::string& string)
	{
		if (mUsesMainStream) mMainStream->output(mStreamName + ": " + string + "\n");
	}

	virtual void fout(unsigned int level, const char* format, ...)
	{		
		if (level > mLogLevel) return;

		va_list vlist;
		va_start(vlist, format);
		char buf[1024]; 
		vsprintf(buf, format, vlist);
		va_end(vlist);

		output(buf);
	}

	virtual void operator<<(const std::string& string) = 0;
};


#endif //LOG_STREAM_H