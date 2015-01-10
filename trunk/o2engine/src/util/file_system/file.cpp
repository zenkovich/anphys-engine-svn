#include "file.h"

#include "file_system.h"
#include "app/application_base_interface.h"

OPEN_O2_NAMESPACE
	
InFile::InFile():
	mOpened(false)
{
}

InFile::InFile( const string& filename, FileType::value type /*= cFileType::FT_FILE*/ ):
	mOpened(false)
{
	open(filename, type);
}

InFile::~InFile()
{
	close();
}

bool InFile::open( const string& filename, FileType::value type /*= cFileType::FT_FILE*/ )
{
	close();

	if (type == FileType::FILE)
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

		const FileSystem::ExtensionsVec extensions = fileSystem()->getExtensions(type);
		for (FileSystem::ExtensionsVec::const_iterator it = extensions.cbegin(); it != extensions.cend(); ++it)
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

bool InFile::close()
{
	if (mOpened)
		mIfstream.close();

	return true;
}

uint32 InFile::readFullData( void *dataPtr )
{
	mIfstream.seekg(0, std::ios::beg);
	mIfstream.seekg(0, std::ios::end);
	uint32 length = (uint32)mIfstream.tellg();
	mIfstream.seekg(0, std::ios::beg);

	mIfstream.read((char*)dataPtr, length);

	return length;
}

void InFile::readData( void *dataPtr, uint32 bytes )
{
	mIfstream.read((char*)dataPtr, bytes);
}

void InFile::setCaretPos( uint32 pos )
{
	mIfstream.seekg(pos, std::ios::beg);
}

uint32 InFile::getCaretPos()
{
	return (uint32)mIfstream.tellg();
}

uint32 InFile::getDataSize()
{
	mIfstream.seekg(0, std::ios::beg);
	mIfstream.seekg(0, std::ios::end);
	return (long unsigned int)mIfstream.tellg();
}

const string& InFile::getFilename() const
{
	return mFilename;
}

bool InFile::isOpened() const
{
	return mOpened;
}

//cOutFile
OutFile::OutFile():
	mOpened(false)
{

}

OutFile::OutFile( const string& filename, FileType::value type /*= cFileType::FT_FILE*/ ):
	mOpened(false)
{
	open(filename, type);
}

OutFile::~OutFile()
{
	close();
}

bool OutFile::open( const string& filename, FileType::value type /*= cFileType::FT_FILE*/ )
{
	close();

	string resFilename = filename;
	if (type != FileType::FILE)
	{
		string extensionStr;		
		const FileSystem::ExtensionsVec extensions = fileSystem()->getExtensions(type);

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

bool OutFile::close()
{
	if (mOpened)
		mOfstream.close();

	return true;
}

void OutFile::writeData( const void* dataPtr, uint32 bytes )
{
	mOfstream.write((const char*)dataPtr, bytes);
}

const string& OutFile::getFilename() const
{
	return mFilename;
}

bool OutFile::isOpened() const
{
	return mOpened;
}

CLOSE_O2_NAMESPACE