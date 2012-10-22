#include "../../engine/engine_options.h"
#ifdef PLATFORM_WIN32

#ifndef FILE_WIN_H
#define FILE_WIN_H

#include "file_base_interface.h"

#include <fstream>

struct cFileBase:public cFileBaseInterface
{
	/*struct cFileBaseInterface
	{
		enum FileType { FT_INPUT = 0, FT_OUTPUT };

		FileType	mFileType;
	}; */
	
	std::ifstream mInFile;		/**< Файл чтения. */
	std::ofstream mOutFile;		/**< Файл записи. */
	
	cFileBase();	
	cFileBase(const char* fileName, FileType type = FT_INPUT);
	~cFileBase();

	bool openRead(const char* fileName);
	bool openWrite(const char* fileName);

	void close();

	long unsigned int readFullData(void *dataPtr);
	void writeData(void* dataPtr, long unsigned int size);
	void readData(void *dataPtr, long unsigned int bytes);

	void setCarerPos(int pos);
	int getCaretPos();

	long unsigned int getDataSize();
};

#endif //FILE_WIN_H

#endif //PLATFORM_WIN32
