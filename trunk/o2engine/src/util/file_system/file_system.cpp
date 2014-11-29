#include "file_system.h"
#include <Windows.h>
#include "util/time_utils.h"
#include "util/build_system/build_system.h"

OPEN_O2_NAMESPACE

cFileSystem::cFileSystem()
{
	mExtensions[cFileType::IMAGE].push_back("png");
	mExtensions[cFileType::IMAGE].push_back("jpg");
	mExtensions[cFileType::IMAGE].push_back("dds");
	mExtensions[cFileType::IMAGE].push_back("pvr");
	
	mExtensions[cFileType::CONFIG].push_back("xml");
	mExtensions[cFileType::CONFIG].push_back("txt");
	mExtensions[cFileType::CONFIG].push_back("cfg");
	
	mExtensions[cFileType::ATLAS].push_back("atlas");

	mResourcePath = ASSETS_BUILDED_PATH;
}

cFileSystem::~cFileSystem()
{
	cBuildSystem::deinitializeSingleton();
}

const cFileSystem::ExtensionsVec& cFileSystem::getExtensions( cFileType::value fileType ) const
{
	return mExtensions.at(fileType);
}

string cFileSystem::getFilePathByExt(const string& path, cFileType::value fileType) const
{
	ExtensionsVec exts = getExtensions(fileType);
	FOREACH(ExtensionsVec, exts, ext)
	{
		string fullPath = path + "." + (*ext);
		if (isFileExist(fullPath))
			return fullPath;
	}

	return path + "." + exts[0];
}

const string& cFileSystem::getResourcePath() const
{
	return mResourcePath;
}

cPathInfo cFileSystem::getPathInfo(const string& path) const
{
	cPathInfo res;
	res.mPath = path;

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile((path + "/*").c_str(), &f);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
				continue;

			if (f.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				res.mPaths.add(getPathInfo(path + "/" + f.cFileName));
			else
				res.mFiles.add(getFileInfo(path + "/" + f.cFileName));
		} 
		while(FindNextFile(h, &f));
	}
	else
		gLog->error("Failed getPathInfo: Error opening directory %s", path.c_str());

	FindClose(h);
	
	return res;
}

bool cFileSystem::copyFile(const string& source, const string& dest) const
{
	deleteFile(dest);
	createDirectory(extractPath(dest));
	return CopyFile(source.c_str(), dest.c_str(), TRUE) == TRUE;
}

bool cFileSystem::deleteFile(const string& file) const
{
	return DeleteFile(file.c_str()) == TRUE;
}

bool cFileSystem::moveFile(const string& source, const string& dest) const
{
	return MoveFile(source.c_str(), dest.c_str()) == TRUE;
}

cFileInfo cFileSystem::getFileInfo(const string& path) const
{
	cFileInfo res;
	res.mPath = "invalid_file";

	FILETIME creationTime, lastAccessTime, lastWriteTime;
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
		return res;

	if (!GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime)) 
	{		
		CloseHandle(hFile);
		return res;
	}
	
	SYSTEMTIME stUTC, stLocal;

	FileTimeToSystemTime(&creationTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	res.mCreatedDate = WideTime(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

	FileTimeToSystemTime(&lastAccessTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	res.mAccessDate = WideTime(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

	FileTimeToSystemTime(&lastWriteTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	res.mEditDate = WideTime(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

	res.mPath = path;
	string extension = path.substr(path.rfind(".") + 1);
	res.mFileType = cFileType::FILE;

	FOREACH_CONST(ExtensionsMap, mExtensions, iext)
	{
		if (FIND(iext->second, extension) != iext->second.end())
		{
			res.mFileType = iext->first;
			break;
		}
	}
	
	DWORD dwSizeHigh=0, dwSizeLow=0;
	dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
    res.mSize = (dwSizeHigh * (MAXDWORD+1)) + dwSizeLow;
	res.mCheckSumm = res.mSize;

	CloseHandle(hFile);

	return res;
}

bool cFileSystem::createDirectory(const string& path, bool recursive /*= true*/) const
{
	if (isDirectoryExist(path))
		return true;

	if (!recursive)
		return CreateDirectory(path.c_str(), NULL) == TRUE;

	if (CreateDirectory(path.c_str(), NULL) == TRUE)
		return true;

	string extrPath = extractPath(path);
	if (extrPath == path)
		return false;

	return createDirectory(extrPath, true);
}

bool cFileSystem::removeDirectory(const string& path, bool recursive /*= true*/) const
{
	if (!isDirectoryExist(path))
		return false;

	if (!recursive)
		return RemoveDirectory(path.c_str()) == TRUE;

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile((path + "/*").c_str(), &f);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
				continue;

			if (f.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				removeDirectory(path + "/" + f.cFileName, true);
			else
				deleteFile(path + "/" + f.cFileName);
		} 
		while(FindNextFile(h, &f));
	}

	FindClose(h);

	return RemoveDirectory(path.c_str()) == TRUE;
}

bool cFileSystem::isDirectoryExist(const string& path) const
{
	DWORD tp = GetFileAttributes(path.c_str());
	
	if (tp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (tp & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

bool cFileSystem::isFileExist(const string& path) const
{
	DWORD tp = GetFileAttributes(path.c_str());
	
	if (tp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (tp & FILE_ATTRIBUTE_DIRECTORY)
		return false;

	return true;
}

CLOSE_O2_NAMESPACE
