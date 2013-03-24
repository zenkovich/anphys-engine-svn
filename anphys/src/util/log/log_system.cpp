#include "log_system.h"
#include "log_stream.h"
#include "../memory/mem_utils.h"

#include "log_stream_in_file.h"
#include "log_stream_console.h"
#include "log_stream_console_and_file.h"

cLogSystem *gLogSystem;
cLogStreamInConsoleAndFile* gLog;

cLogSystem::cLogSystem(const std::string& mainStreamFile)
{
	gLog = static_cast<cLogStreamInConsoleAndFile*>(setMainStream(new cLogStreamInConsoleAndFile(mainStreamFile)));
}

cLogSystem::~cLogSystem()
{
	for (StreamsMap::iterator it = mStreams.begin(); it != mStreams.end(); it++)
		safe_release((*it).second);
}

cLogStream* cLogSystem::addStream(cLogStream* newStream, const std::string& streamName, bool usesMainStream)
{
	mStreams[streamName] = newStream;
	newStream->mStreamName = adjustStrSize(streamName, 20, 2, '-');

	if (usesMainStream && mMainStream != NULL)
	{
		mStreams[streamName]->mMainStream = mMainStream;
		mStreams[streamName]->mUsesMainStream = true;
	}

	return mStreams[streamName];
}

cLogStream* cLogSystem::setMainStream(cLogStream* mainStream)
{
	mStreams["mainStream"] = mainStream;
	mMainStream = mStreams["mainStream"];

	return mStreams["mainStream"];
}

cLogStream* cLogSystem::getStream(const std::string& streamName)
{
	StreamsMap::iterator it = mStreams.find(streamName);

	if (it != mStreams.end()) return it->second;
	else return mMainStream;
}

bool cLogSystem::removeStream(cLogStream* stream)
{	
	for (StreamsMap::iterator it = mStreams.begin(); it != mStreams.end(); it++)
	{
		if (it->second == stream)
		{
			it = mStreams.erase(it);

			return true;
		}
	}	

	safe_release(stream);

	return false;
}

bool cLogSystem::removeStream(const std::string& streamName)
{
	StreamsMap::iterator it = mStreams.find(streamName);

	if (it != mStreams.end()) 
	{
		safe_release(it->second);
		mStreams.erase(it);

		return true;
	}

	return false;
}

void cLogSystem::output(const std::string& streamName, const std::string& string)
{
	cLogStream* stream = getStream(streamName);
	if (stream == NULL) return;

	stream->output(string);
}

cLogSystem* createLogSystem(const std::string& fileName)
{
	return new cLogSystem(fileName);
}