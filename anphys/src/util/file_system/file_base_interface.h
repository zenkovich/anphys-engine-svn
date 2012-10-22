#ifndef FILE_BASE_INTERFACE_H
#define FILE_BASE_INTERFACE_H


/** Èםעונפויס פאיכא. */
struct cFileBaseInterface
{
	enum FileType { FT_INPUT = 0, FT_OUTPUT, FT_NONE };

	FileType	mFileType;

	virtual bool openRead(const char* fileName) { return false; }
	virtual bool openWrite(const char* fileName) { return false; }

	virtual void close() {}

	virtual long unsigned int readFullData(void *dataPtr) { return 0; }
	virtual void writeData(void* dataPtr, long unsigned int size) {}
	virtual void readData(void *dataPtr, long unsigned int bytes) {}

	virtual void setCarerPos(int pos) {}
	virtual int getCaretPos() { return 0; }

	virtual long unsigned int getDataSize() { return 0; }
};


#endif //FILE_BASE_INTERFACE_H