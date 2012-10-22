#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

#include <map>
#include <string>

#include "../other/string_utils.h"
#include "log_stream_console_and_file.h"

struct cLogStream;

/** ��������� ���-�������. ������ � ���� ������, ��������� �� ������� �����. */
struct cLogSystem
{
	typedef std::map<std::string, cLogStream*> StreamsMap;  /**< ��� ������ - ��������� �� ����. */

	StreamsMap mStreams;       /**< ������ �������. */
	cLogStream* mMainStream;   /**< ��������� �� ������� �����.  ���� �� ������������, �� = 0. */
	
	cLogSystem(): mMainStream(NULL) {}
	cLogSystem(const std::string& mainStreamFile);
	~cLogSystem();

	/** �������� �����. */
	cLogStream* addStream(cLogStream* newStream, const std::string& streamName, bool usesMainStream = true);

	/** ���������� ������� �����. */
	cLogStream* setMainStream(cLogStream* mainStream);

	/** �������� ����� �� �����. */
	cLogStream* getStream(const std::string& streamName);

	/** ������� �����. */
	bool removeStream(cLogStream* stream);

	/** ������� �����. */
	bool removeStream(const std::string& streamName);

	void output(const std::string& streamName, const std::string& string);
};

/** ������� ���-�������. */
cLogSystem* createLogSystem(const std::string& fileName);

extern cLogSystem *gLogSystem;
extern cLogStreamInConsoleAndFile* gLog;





#endif //LOG_SYSTEM_H