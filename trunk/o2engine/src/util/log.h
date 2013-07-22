#ifndef LOG_H
#define LOG_H

#include "public.h"
#include "log/log_stream.h"

OPEN_O2_NAMESPACE

/** Global log stream. Use it for putting data in log. */
extern cLogStream* gLog;

/** Initializing basic log streams.
 ** @param fileName - output global log file. */
void initializeFileConsoleLog(const std::string& fileName);

/** Puts data into global log with low level. */
#define log(...) gLog->log(__VA_ARGS__);

/** Puts data into global log with hight level. */
#define hlog(...) gLog->hlog(__VA_ARGS__);

CLOSE_O2_NAMESPACE

#endif //LOG_H