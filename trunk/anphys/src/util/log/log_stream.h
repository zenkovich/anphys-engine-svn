#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <string>

/** Поток лог-системы. Хранит в себе указатель на главный поток и флаг, используется ли главный поток. */
struct cLogStream
{
	cLogStream* mMainStream;  /**< Указательна главный поток. Все, что будет выведено в этом поток, будет выводится и в главный. */
	bool mUsesMainStream;     /**< Используется ли главный поток. */
	std::string mStreamName;  /**< Имя потока. */

	cLogStream(): mMainStream(NULL), mUsesMainStream(false) {}
	cLogStream(cLogStream* mainStream): mMainStream(mainStream), mUsesMainStream(true) {}

	/** Функция-вывод. */
	virtual void output(const std::string& string)
	{
		if (mUsesMainStream) mMainStream->output(mStreamName + ": " + string);
	}

	virtual void operator<<(const std::string& string) = 0;
};

#endif //LOG_STREAM_H