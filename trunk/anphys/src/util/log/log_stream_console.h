#ifndef LOG_STREAM_CONSOLE_H
#define LOG_STREAM_CONSOLE_H

#include "../../engine/engine_options.h"
#include "log_stream.h"
#include <string>

#ifdef PLATFORM_WIN32

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <fstream>

/** Поток в консоль. */
struct cLogStreamInConsole:public cLogStream
{
	bool mReady; /**< Готов ли к использованию. */

	//functions
	cLogStreamInConsole(): cLogStream(), mReady(false) { createConsole(); }

	cLogStreamInConsole(cLogStream *mainStream): cLogStream(mainStream), mReady(false) { createConsole(); }

	/** Создает консоль, если еще не создана. */
	void createConsole()
	{
		if ( AllocConsole() )
		{
			 int hCrt = _open_osfhandle((long)
				GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
				*stdout = *(::_fdopen(hCrt, "w"));
			::setvbuf(stdout, NULL, _IONBF, 0);
			*stderr = *(::_fdopen(hCrt, "w"));
			::setvbuf(stderr, NULL, _IONBF, 0);
		}

		mReady = true;
	}

	~cLogStreamInConsole() { FreeConsole(); }

	virtual void output(const std::string& string)
	{
		cLogStream::output(string);

		if (!mReady) return;

		printf(string.c_str());
	}

	virtual void operator<<(const std::string& string)
	{
		output(string);
	}
};

#endif //PLATFORM_WIN


#endif //LOG_STREAM_CONSOLE_H