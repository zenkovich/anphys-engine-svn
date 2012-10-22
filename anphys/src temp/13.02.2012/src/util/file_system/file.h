#ifndef FILE_H
#define FILE_H

#include <string>

struct cFileIdentity;

struct cFile
{
	cFileIdentity* mFileIdentity;

	cFile();
	~cFile();

	bool mReady;
	
	bool openRead(const std::string& fileName);
	bool openWrite(const std::string& fileName);

	void close();

	long unsigned int readData(void *dataPtr);
	void writeData(void* dataPtr, long unsigned int size);

	void setCarerPos(int pos);
	int getCaretPos();
	void readData(void *dataPtr, int bytes);

	long unsigned int getDataSize();
};

#endif //FILE_H