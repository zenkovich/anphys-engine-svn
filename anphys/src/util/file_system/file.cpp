#include "file.h"

cFile::cFile():cFileBase(), mReady(false), mRefCount(0) {}

cFile::~cFile() { mRefCount = 1; close(); }

cFile::cFile(const char* fileName, FileType type):cFileBase(fileName, type) 
{
	if (mFileType != FT_NONE) 
	{
		mReady = true;
		mRefCount = 1;
	}
}

bool cFile::openRead(const char* fileName)
{
	if (cFileBase::openRead(fileName))
	{
		mReady = true;
		mRefCount++;
		return true;
	}

	return false;
}

bool cFile::openWrite(const char* fileName)
{
	if (cFileBase::openWrite(fileName))
	{
		mReady = true;
		mRefCount++;
		return true;
	}

	return false;
}

void cFile::close()
{
	mRefCount--;

	if (mRefCount < 1) cFileBase::close();
}
