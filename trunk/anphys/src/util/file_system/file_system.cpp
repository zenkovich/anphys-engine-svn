#include "file_system.h"

#include "../memory/mem_utils.h"
#include "../log/log_system.h"
#include "../log/log_stream_in_file.h"

#include <algorithm>

DECLARE_SINGLETONE(cFileSystem)

cFileSystem::cFileSystem()
{
	strcpy_s(mFileTypesExtensions[FT_IMAGE][0], "png");
	strcpy_s(mFileTypesExtensions[FT_IMAGE][1], "jpg");
	strcpy_s(mFileTypesExtensions[FT_IMAGE][2], "dds");
	strcpy_s(mFileTypesExtensions[FT_IMAGE][3], "tga");
	strcpy_s(mFileTypesExtensions[FT_IMAGE][4], "img");
	
	strcpy_s(mFileTypesExtensions[FT_SCRIPT][0], "lua");
	strcpy_s(mFileTypesExtensions[FT_SCRIPT][1], "txt");
	strcpy_s(mFileTypesExtensions[FT_SCRIPT][2], "scr");
	strcpy_s(mFileTypesExtensions[FT_SCRIPT][3], "scr");
	strcpy_s(mFileTypesExtensions[FT_SCRIPT][4], "scr");
	
	mLog = static_cast<cLogStreamInFile*>(gLogSystem->addStream(new cLogStreamInFile("file system log.txt"), "fileSystemLog"));
}

cFileSystem::~cFileSystem()
{
	for (FilesList::iterator it = mFiles.begin(); it != mFiles.end(); it++)
	{
		safe_release(it->second);
	}

	mFiles.clear();
}

cFile* cFileSystem::tryOpen(const std::string& fileName, cFile::FileType openingType)
{
	FilesList::iterator fnd = mFiles.begin();
	for(; fnd != mFiles.end(); fnd++)
		if (fnd->first == fileName) break;

	if (fnd == mFiles.end())
	{
		cFile* newFile = new cFile((mResourcesDirectory + fileName).c_str(), openingType);

		if (!newFile->mReady) 
		{
			safe_release(newFile);
			return NULL;
		}

		mFiles[fileName] = newFile;
		return newFile;
	}

	if (openingType != fnd->second->mFileType) 
	{
		return NULL;
	}

	return fnd->second;
}

cFile* cFileSystem::openFile(const std::string& fileName, cFile::FileType openingType, FileType fileType)
{
	if (fileType == FT_FILE) return tryOpen(fileName, openingType);

	cFile* res = NULL;
	int iter = 0;
	do
	{
		char fullName[256];
		sprintf(fullName, "%s.%s", fileName.c_str(), mFileTypesExtensions[fileType][iter]);
		res = tryOpen(fullName, openingType);
		if (res) break;
	}
	while(iter < 5);

	return res;
}

cFile* cFileSystem::openFileRead(const std::string& fileName, FileType fileType)
{
	return openFile(fileName, cFileBaseInterface::FT_INPUT, fileType);
}

cFile* cFileSystem::openFileWrite(const std::string& fileName, FileType fileType)
{
	return openFile(fileName, cFileBaseInterface::FT_OUTPUT, fileType);
}

void cFileSystem::closeFile(cFile* file)
{
	file->close();

	if (file->mRefCount < 1)
	{
		FilesList::iterator fnd = mFiles.begin();
		for(; fnd != mFiles.end(); fnd++)
			if (fnd->second == file) break;

		if (fnd != mFiles.end())
		{
			safe_release(fnd->second);
			mFiles.erase(fnd);
		}
		else
		{
			mLog->output(formatStr("WARNING: Can't find file %x in files map. Not bad but not good.", file));
			safe_release(file);
		}
	}
}

bool cFileSystem::isExist(const std::string& fileName)
{
	std::ifstream fin;
	fin.open(fileName);
	bool opened = (bool)fin.is_open();
	fin.close();

	return opened;
}
