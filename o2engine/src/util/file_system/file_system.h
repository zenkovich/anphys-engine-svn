#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <map>

#include "public.h"
#include "file.h"
#include "file_info.h"
#include "util/string.h"

OPEN_O2_NAMESPACE
	
/** File system, singleton. Containing basic resource path, extension strings. */
class cFileSystem
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

	/** Returns resource path. */
	const string& getResourcePath() const;

	/** Returns vector of extension strings for extension type. */
	const ExtensionsVec& getExtensions(cFileType::value fileType) const;

	/** Returns full file path with extension by file type. */
	string getFilePathByExt(const string& path, cFileType::value fileType) const;

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

	/** Returns true if specified file exist. */
	bool isFileExist(const string& path) const;
};

CLOSE_O2_NAMESPACE

#endif //FILE_SYSTEM_H