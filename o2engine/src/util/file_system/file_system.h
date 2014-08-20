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

	/** Checking assets building. */
	void checkAssetsBuilding() const;

	/** Returns info of paths and files of specified path. */
	cPathInfo getPathInfo(const string& path) const;

	/** Returns file info. */
	cFileInfo getFileInfo(const string& path) const;

	/** Copying file. */
	bool copyFile(const string& source, const string& dest) const;

	/** Deletion file. */
	bool deleteFile(const string& file) const;

	/** Moving file. */
	bool moveFile(const string& source, const string& dest) const;

	/** Creates folder. */
	bool createDirectory(const string& path, bool recursive = true) const;

	/** Removes directory. */
	bool removeDirectory(const string& path, bool recursive = true) const;

	/** Returns true if specified directory exist. */
	bool isDirectoryExist(const string& path) const;
};

#define getFileSystem() cFileSystem::instance()

CLOSE_O2_NAMESPACE

#endif //FILE_SYSTEM_H