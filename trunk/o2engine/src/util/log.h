#ifndef LOG_H
#define LOG_H

#include "public.h"
#include "log/log_stream.h"

OPEN_O2_NAMESPACE

/** Global log stream. Use it for putting data in log. */
extern cLogStream* gLog;

CLOSE_O2_NAMESPACE

/** Puts data into global log with low level. */
#define llog(str, ...) o2::gLog->out(str, __VA_ARGS__);

/** Puts data into global log with hight level. */
#define hlog(str, ...) o2::gLog->hout(str, __VA_ARGS__);

/** Puts error message into global log. */
#define logError(str, ...) o2::gLog->error(str, __VA_ARGS__);

/** Puts warning message into global log. */
#define logWarning(str, ...) o2::gLog->warning(str, __VA_ARGS__);

#endif //LOG_H