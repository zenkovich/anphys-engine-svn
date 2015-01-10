#ifndef CONSOLE_LOG_STREAM_H
#define CONSOLE_LOG_STREAM_H

#include "log_stream.h"

OPEN_O2_NAMESPACE

class ConsoleLogStream:public LogStream
{
public:
	ConsoleLogStream(uint8 level);
	ConsoleLogStream(const string& id, uint8 level);
	~ConsoleLogStream();

protected:
	void outStrEx(const string& str);
	void initConsole();
};

CLOSE_O2_NAMESPACE

#endif //CONSOLE_LOG_STREAM_H