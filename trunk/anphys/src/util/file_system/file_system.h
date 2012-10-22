#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>
#include <map>
#include <vector>

#include "file.h"

struct cLogStreamInFile;

struct cFileSystem
{
	typedef std::map<std::string, cFile*> FilesList;
	enum FileType { FT_IMAGE = 0, FT_SCRIPT, FT_FILE };
	enum { kFileTypesCount = 2 };

	char				mFileTypesExtensions[kFileTypesCount][5][5];
	char				mApplicationDirectory[256];
	FilesList			mFiles;

	cLogStreamInFile*	mFileSystemLog;


	cFileSystem();
	~cFileSystem();

	cFile* openFile(const std::string& fileName, cFile::FileType openingType = cFile::FT_INPUT, FileType fileType = FT_FILE );
	cFile* openFileRead(const std::string& fileName, FileType fileType = FT_FILE );
	cFile* openFileWrite(const std::string& fileName, FileType fileType = FT_FILE );

	void closeFile(cFile* file);

	bool isExist(const std::string& fileName);

	cFile* tryOpen(const std::string& fileName, cFile::FileType openingType);
};


#endif //FILE_SYSTEM_H