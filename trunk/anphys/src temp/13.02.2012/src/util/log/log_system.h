#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

#include <map>
#include <string>

#include "../other/string_utils.h"
#include "log_stream_console_and_file.h"

struct cLogStream;

/** Структура лог-системы. Хранит в себе потоки, указатель на главный поток. */
struct cLogSystem
{
	typedef std::map<std::string, cLogStream*> StreamsMap;  /**< Имя потока - указатель на него. */

	StreamsMap mStreams;       /**< Список потоков. */
	cLogStream* mMainStream;   /**< Указатель на главный поток.  Если не используется, то = 0. */
	
	cLogSystem(): mMainStream(NULL) {}
	cLogSystem(const std::string& mainStreamFile);
	~cLogSystem();

	/** Добавить поток. */
	cLogStream* addStream(cLogStream* newStream, const std::string& streamName, bool usesMainStream = true);

	/** Установить главный поток. */
	cLogStream* setMainStream(cLogStream* mainStream);

	/** ПОлучить поток по имени. */
	cLogStream* getStream(const std::string& streamName);

	/** Удалить поток. */
	bool removeStream(cLogStream* stream);

	/** Удалить поток. */
	bool removeStream(const std::string& streamName);

	void output(const std::string& streamName, const std::string& string);
};

/** СОздать лог-систему. */
cLogSystem* createLogSystem(const std::string& fileName);

extern cLogSystem *gLogSystem;
extern cLogStreamInConsoleAndFile* gLog;





#endif //LOG_SYSTEM_H