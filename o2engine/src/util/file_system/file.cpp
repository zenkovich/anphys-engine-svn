#include "file.h"

#include "file_system.h"
#include "app/application_base_interface.h"

OPEN_O2_NAMESPACE
	
cInFile::cInFile():
	mOpened(false)
{
}

cInFile::cInFile( const string& filename, cFileType::value type /*= cFileType::FT_FILE*/ ):
	mOpened(false)
{
	open(filename, type);
}

cInFile::~cInFile()
{
	close();
}

bool cInFile::open( const string& filename, cFileType::value type /*= cFileType::FT_FILE*/ )
{
	close();

	if (type == cFileType::FT_FILE)
	{
		mIfstream.open(filename.c_str(), std::ios::binary);

		if (!mIfstream.is_open())
		{
			return false;
		}
	}
	else
	{
		bool success = false;

		const cFileSystem::ExtensionsVec extensions = fileSystem()->getExtensions(type);
		for (cFileSystem::ExtensionsVec::const_iterator it = extensions.cbegin(); it != extensions.cend(); ++it)
		{
			string resFilename = fileSystem()->getResourcePath() + filename + "." + (*it);

			mIfstream.open(resFilename.c_str(), std::ios::binary);

			if (mIfstream.is_open())
			{
				success = true;
				break;
			}
		}

		if (!success)
			return false;
	}

	mOpened = true;
	mFilename = filename;
	return true;
}

bool cInFile::close()
{
	if (mOpened)
		mIfstream.close();

	return true;
}

uint32 cInFile::readFullData( void *dataPtr )
{
	mIfstream.seekg(0, std::ios::beg);
	mIfstream.seekg(0, std::ios::end);
	uint32 length = (uint32)mIfstream.tellg();
	mIfstream.seekg(0, std::ios::beg);

	mIfstream.read((char*)dataPtr, length);

	return length;
}

void cInFile::readData( void *dataPtr, uint32 bytes )
{
	mIfstream.read((char*)dataPtr, bytes);
}

void cInFile::setCaretPos( uint32 pos )
{
	mIfstream.seekg(pos, std::ios::beg);
}

uint32 cInFile::getCaretPos()
{
	return (uint32)mIfstream.tellg();
}

uint32 cInFile::getDataSize()
{
	mIfstream.seekg(0, std::ios::beg);
	mIfstream.seekg(0, std::ios::end);
	return (long unsigned int)mIfstream.tellg();
}

const string& cInFile::getFilename() const
{
	return mFilename;
}

bool cInFile::isOpened() const
{
	return mOpened;
}

//cOutFile
cOutFile::cOutFile():
	mOpened(false)
{

}

cOutFile::cOutFile( const string& filename, cFileType::value type /*= cFileType::FT_FILE*/ ):
	mOpened(false)
{
	open(filename, type);
}

cOutFile::~cOutFile()
{
	close();
}

bool cOutFile::open( const string& filename, cFileType::value type /*= cFileType::FT_FILE*/ )
{
	close();

	string resFilename = filename;
	if (type != cFileType::FT_FILE)
	{
		string extensionStr;		
		const cFileSystem::ExtensionsVec extensions = fileSystem()->getExtensions(type);

		if (extensions.size() > 0)
			extensionStr = extensions[0];

		resFilename = fileSystem()->getResourcePath() + filename + "." + extensionStr;
	}

	mOfstream.open(resFilename.c_str(), std::ios::binary);

	if (!mOfstream.is_open())
	{
		return false;
	}	

	mOpened = true;
	mFilename = filename;
	return true;
}

bool cOutFile::close()
{
	if (mOpened)
		mOfstream.close();

	return true;
}

void cOutFile::writeData( const void* dataPtr, uint32 bytes )
{
	mOfstream.write((const char*)dataPtr, bytes);
}

const string& cOutFile::getFilename() const
{
	return mFilename;
}

bool cOutFile::isOpened() const
{
	return mOpened;
}

CLOSE_O2_NAMESPACE