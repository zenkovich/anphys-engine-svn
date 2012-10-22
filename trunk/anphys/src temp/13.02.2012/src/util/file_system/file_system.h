#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>
#include <map>
#include <vector>

struct cFile;

struct cFileSystem
{
	cFileSystem();
	~cFileSystem();

	std::string mApplicationDirectory;

	typedef std::map<std::string, cFile*> FilesList;
	FilesList mFiles;

	cFile* openFile(const std::string& fileName);
	void closeFile(cFile* file);
};


#endif //FILE_SYSTEM_H