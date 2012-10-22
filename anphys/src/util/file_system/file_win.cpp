#include "../../engine/engine_options.h"
#ifdef PLATFORM_WIN32

#include "file_win.h"


cFileBase::cFileBase() { mFileType = FT_NONE; }

cFileBase::cFileBase(const char* fileName, FileType type)
{
	if (type == FT_INPUT)	openRead(fileName);
	else					openWrite(fileName);
}

cFileBase::~cFileBase() { close(); }

bool cFileBase::openRead(const char* fileName)
{
	if (mFileType != FT_NONE) close();

	mInFile.open(fileName);
	
	if (!mInFile.is_open()) return false;
	
	mFileType = FT_INPUT;

	return true;
}

bool cFileBase::openWrite(const char* fileName)
{
	if (mFileType != FT_NONE) close();

	mOutFile.open(fileName);
	
	if (!mOutFile.is_open()) return false;
	
	mFileType = FT_OUTPUT;

	return true;
}

void cFileBase::close()
{
	if (mFileType == FT_INPUT) mInFile.close();
	if (mFileType == FT_OUTPUT) mOutFile.close();
}

long unsigned int cFileBase::readFullData(void *dataPtr)
{
	if (mFileType != FT_INPUT) return 0;

	mInFile.seekg(0, std::ios::beg);
	mInFile.seekg(0, std::ios::end);
	long unsigned int length = (long unsigned int)mInFile.tellg();
	mInFile.seekg(0, std::ios::beg);

	mInFile.read((char*)dataPtr, length);

	return length;
}

void cFileBase::writeData(void* dataPtr, long unsigned int size)
{
	if (mFileType != FT_OUTPUT) return;

	mOutFile.write((char*)dataPtr, size);
}

void cFileBase::readData(void *dataPtr, long unsigned int bytes)
{
	if (mFileType != FT_INPUT) return;

	mInFile.read((char*)dataPtr, bytes);
}

void cFileBase::setCarerPos(int pos)
{
	if (mFileType ==  FT_INPUT) mInFile.seekg(pos, std::ios::beg);
}

int cFileBase::getCaretPos()
{
	if (mFileType ==  FT_INPUT) return (int)mInFile.tellg();

	return 0;
}

long unsigned int cFileBase::getDataSize()
{
	if (mFileType != FT_INPUT) return 0;

	mInFile.seekg(0, std::ios::beg);
	mInFile.seekg(0, std::ios::end);
	return (long unsigned int)mInFile.tellg();
}

#endif //PLATFORM_WIN