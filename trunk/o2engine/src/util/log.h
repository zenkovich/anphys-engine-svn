#ifndef LOG_H
#define LOG_H

#include "log/log_stream.h"
#include "util/smart_ptrs.h"

OPEN_O2_NAMESPACE

/** Global log stream. Use it for putting data in log. */
extern cLogStream* gLog;


/** Puts data into global log with low level. */
#define llog(str, ...) gLog->out(str, __VA_ARGS__);

/** Puts data into global log with hight level. */
#define hlog(str, ...) gLog->hout(str, __VA_ARGS__);

/** Puts error message into global log. */
#define logError(str, ...) gLog->error(str, __VA_ARGS__);

/** Puts warning message into global log. */
#define logWarning(str, ...) gLog->warning(str, __VA_ARGS__);

CLOSE_O2_NAMESPACE

#endif //LOG_H