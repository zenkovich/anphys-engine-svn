#include "console_log_stream.h"

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <wincon.h>

OPEN_O2_NAMESPACE

ConsoleLogStream::ConsoleLogStream(uint8 level):
	LogStream()
{
	setLevel(level);
	if (level > 0)
		initConsole();
}

ConsoleLogStream::ConsoleLogStream( const string& id, uint8 level ):
	LogStream(id)
{
	setLevel(level);
	if (level > 0)
		initConsole();
}

ConsoleLogStream::~ConsoleLogStream()
{
	FreeConsole();
}

void ConsoleLogStream::outStrEx( const string& str )
{
	printf(str.c_str());
	printf("\n");	
}

void ConsoleLogStream::initConsole()
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
}

CLOSE_O2_NAMESPACE