#ifndef LOG_STREAM_IN_FILE_H
#define LOG_STREAM_IN_FILE_H

#include "../../engine/engine_options.h"
#include "log_stream.h"
#include <string>

#ifdef PLATFORM_WIN32

#include <fstream>

/** Поток в файл. */
struct cLogStreamInFile:public cLogStream
{
	std::ofstream mTextFile;  /**< Файл. */
	bool mReady;              /**< Готов ли к использованию. */

	//functions
	cLogStreamInFile(): cLogStream(), mReady(false) {}

	cLogStreamInFile(cLogStream *mainStream): cLogStream(mainStream), mReady(false) {}

	cLogStreamInFile(const std::string& fileName, cLogStream *mainStream): cLogStream(mainStream)
	{
		mTextFile.open(fileName.c_str());
		mReady = mTextFile.is_open();
	}

	cLogStreamInFile(const std::string& fileName): cLogStream()
	{
		mTextFile.open(fileName.c_str());
		mReady = mTextFile.is_open();
	}

	~cLogStreamInFile() { if (mTextFile && mTextFile.is_open()) mTextFile.close(); }

	/** Открыть файл. */
	void loadFile(const std::string& fileName)
	{
		if (mReady) mTextFile.close();
		
		mTextFile.open(fileName.c_str());
		mReady = mTextFile.is_open();
	}

	virtual void output(const std::string& string)
	{
		cLogStream::output(string);

		if (!mReady) return;

		mTextFile << string.c_str();
	}

	virtual void operator<<(const std::string& string)
	{
		output(string);
	}

};

#endif //PLATFORM_WIN

#endif //LOG_STREAM_IN_FILE_H