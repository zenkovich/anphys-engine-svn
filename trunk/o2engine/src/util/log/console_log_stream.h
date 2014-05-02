#ifndef CONSOLE_LOG_STREAM_H
#define CONSOLE_LOG_STREAM_H

#include "log_stream.h"

OPEN_O2_NAMESPACE

class cConsoleLogStream:public cLogStream
{
public:
	cConsoleLogStream(uint8 level);
	cConsoleLogStream(const string& id, uint8 level);
	~cConsoleLogStream();

protected:
	void outStrEx(const string& str);
	void initConsole();
};

CLOSE_O2_NAMESPACE

#endif //CONSOLE_LOG_STREAM_H