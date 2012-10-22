#include "../../engine/engine_options.h"
#ifdef PLATFORM_WIN32


#include "file_win.h"
#include "file.h"

/*

struct cFileIdentity
{
	std::ifstream mInFile;
	std::ofstream mOutFile;

	bool mIsInput;
};
*/
cFile::cFile() { mFileIdentity = new cFileIdentity(); }
cFile::~cFile() { delete mFileIdentity; }

bool cFile::openRead(const std::string& fileName)
{
	mFileIdentity->mInFile.open(fileName.c_str());
	mFileIdentity->mIsInput = true;

	mReady = (bool)mFileIdentity->mInFile.is_open();

	return mReady;
}

bool cFile::openWrite(const std::string& fileName)
{
	mFileIdentity->mOutFile.open(fileName.c_str());
	mFileIdentity->mIsInput = false;

	mReady = (bool)mFileIdentity->mOutFile.is_open();

	return mReady;
}
	
void cFile::close()
{
	if (!mReady) return;

	if (mFileIdentity->mIsInput) mFileIdentity->mInFile.close();
	else                         mFileIdentity->mOutFile.close();
}

long unsigned int cFile::readData(void *dataPtr)
{
	if (!mFileIdentity->mInFile || !mReady) return 0;
	
	mFileIdentity->mInFile.seekg(0, std::ios::beg);
	mFileIdentity->mInFile.seekg(0, std::ios::end);
	long unsigned int length = mFileIdentity->mInFile.tellg();
	mFileIdentity->mInFile.seekg (0, std::ios::beg);

	mFileIdentity->mInFile.read((char*)dataPtr, length);

	return length;
}

void cFile::writeData(void* dataPtr, long unsigned int size)
{
	if (mFileIdentity->mInFile || !mReady) return;

	mFileIdentity->mOutFile.write((char*)dataPtr, size);
}

void cFile::setCarerPos(int pos)
{
	if (!mReady) return;

	if (mFileIdentity->mIsInput) mFileIdentity->mInFile.seekg(pos, std::ios::beg);
}

int cFile::getCaretPos()
{
	if (!mReady) return -1;

	if (mFileIdentity->mIsInput) return mFileIdentity->mInFile.tellg();

	return -1;
}

void cFile::readData(void *dataPtr, int bytes)
{
	if (!mFileIdentity->mInFile || !mReady) return;

	mFileIdentity->mInFile.read((char*)dataPtr, bytes);
}

long unsigned int cFile::getDataSize()
{
	if (!mFileIdentity->mInFile || !mReady) return 0;
	
	mFileIdentity->mInFile.seekg(0, std::ios::beg);
	mFileIdentity->mInFile.seekg(0, std::ios::end);
	long unsigned int length = mFileIdentity->mInFile.tellg();

	return length;
}


#endif //PLATFORM_WIN