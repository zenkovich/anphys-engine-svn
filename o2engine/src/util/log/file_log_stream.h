#ifndef FILE_LOG_STREAM_H
#define FILE_LOG_STREAM_H

#include "log_stream.h"

OPEN_O2_NAMESPACE

class OutFile;

class FileLogStream:public LogStream
{
	OutFile* mOutFile;

public:
	FileLogStream(uint8 level, const string& fileName);
	FileLogStream(const string& id, uint8 level, const string& fileName);
	~FileLogStream();

protected:
	void outStrEx(const string& str);
	void outErrorEx(const string& srt);
	void outWarningEx(const string& srt);
	void openFile(const string& fileName);
};

CLOSE_O2_NAMESPACE

#endif //FILE_LOG_STREAM_H