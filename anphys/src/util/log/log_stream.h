#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <string>
#include <cstdarg>

/** ����� ���-�������. ������ � ���� ��������� �� ������� ����� � ����, ������������ �� ������� �����. */
struct cLogStream
{
	cLogStream* mMainStream;  /**< ����������� ������� �����. ���, ��� ����� �������� � ���� �����, ����� ��������� � � �������. */
	bool mUsesMainStream;     /**< ������������ �� ������� �����. */
	std::string mStreamName;  /**< ��� ������. */

	cLogStream(): mMainStream(NULL), mUsesMainStream(false) {}
	cLogStream(cLogStream* mainStream): mMainStream(mainStream), mUsesMainStream(true) {}

	/** �������-�����. */
	virtual void output(const std::string& string)
	{
		if (mUsesMainStream) mMainStream->output(mStreamName + ": " + string);
	}

	virtual void foutput(const char* format, ...)
	{		
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