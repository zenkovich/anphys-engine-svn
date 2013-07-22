#ifndef LOG_H
#define LOG_H

#include "public.h"
#include "log/log_stream.h"

OPEN_O2_NAMESPACE

extern cLogStream* gLog;

void initializeFileConsoleLog(const std::string& fileName);

#define log(...) gLog->log(__VA_ARGS__);
#define hlog(...) gLog->hlog(__VA_ARGS__);

CLOSE_O2_NAMESPACE

#endif //LOG_H