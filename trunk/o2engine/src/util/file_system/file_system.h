#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <vector>
#include <map>

#include "public.h"
#include "util/singleton.h"
#include "file.h"

OPEN_O2_NAMESPACE
	
class cFileSystem:public cSingleton<cFileSystem>
{
public:
	typedef std::vector<std::string> ExtensionsVec;

private:
	std::string                               mResourcePath;
	std::map<cFileType::value, ExtensionsVec> mExtensions;

public:
	cFileSystem();
	~cFileSystem();

	void                 setResourcePath(const std::string& path);
	const std::string&   getResourcePath() const;

	const ExtensionsVec& getExtensions(cFileType::value fileType) const;
};

#define getFileSystem() cFileSystem::instance()

CLOSE_O2_NAMESPACE

#endif //FILE_SYSTEM_H