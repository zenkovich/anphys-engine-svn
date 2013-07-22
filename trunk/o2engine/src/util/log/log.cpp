#include "../log.h"

#include "engine_settings.h"
#include "file_log_stream.h"
#include "console_log_stream.h"

OPEN_O2_NAMESPACE

void initializeFileConsoleLog( const std::string& fileName )
{
	cFileLogStream* fileLog = new cFileLogStream(BASIC_LOG_LEVEL, fileName);
	gLog = new cConsoleLogStream(BASIC_LOG_LEVEL);
	fileLog->bindStream(gLog);
}

CLOSE_O2_NAMESPACE