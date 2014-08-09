#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <map>

#include "public.h"
#include "util/singleton.h"
#include "file.h"
#include "file_info.h"
#include "util/string.h"

OPEN_O2_NAMESPACE
	
/** File system, singleton. Containing basic resource path, extension strings. */
class cFileSystem:public cSingleton<cFileSystem>
{
public:
	typedef vector<string> ExtensionsVec;
	typedef std::map<cFileType::value, ExtensionsVec> ExtensionsMap;

private:
	string        mResourcePath; /**< Basic application resources path. */
	ExtensionsMap mExtensions;   /**< Extensions strings map. */

public:
	cFileSystem();
	~cFileSystem();

	/** Sets resource path. */
	void setResourcePath(const string& path);

	/** Returns resource path. */
	const string& getResourcePath() const;

	/** Returns vector of extension strings for extension type. */
	const ExtensionsVec& getExtensions(cFileType::value fileType) const;

	/** Returns info of paths and files of specified path. */
	cPathInfo getPathInfo(const string& path) const;

	/** Copying file. */
	bool copyFile(const string& source, const string& dest) const;

	/** Deletion file. */
	bool deleteFile(const string& file) const;

	/** Moving file. */
	bool moveFile(const string& source, const string& dest) const;
};

#define getFileSystem() cFileSystem::instance()

CLOSE_O2_NAMESPACE

#endif //FILE_SYSTEM_H