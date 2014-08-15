#include "file_system.h"
#include <Windows.h>
#include "util\time_utils.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cFileSystem);

cFileSystem::cFileSystem()
{
	mExtensions[cFileType::FT_IMAGE].push_back("png");
	mExtensions[cFileType::FT_IMAGE].push_back("jpg");
	mExtensions[cFileType::FT_IMAGE].push_back("dds");
	mExtensions[cFileType::FT_IMAGE].push_back("pvr");
	
	mExtensions[cFileType::FT_CONFIG].push_back("xml");
	mExtensions[cFileType::FT_CONFIG].push_back("txt");
	mExtensions[cFileType::FT_CONFIG].push_back("cfg");
}

cFileSystem::~cFileSystem()
{
}

void cFileSystem::setResourcePath( const string& path )
{
	mResourcePath = path;
}

const cFileSystem::ExtensionsVec& cFileSystem::getExtensions( cFileType::value fileType ) const
{
	return mExtensions.at(fileType);
}

const string& cFileSystem::getResourcePath() const
{
	return mResourcePath;
}

cPathInfo cFileSystem::getPathInfo(const string& path) const
{
	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile("./*", &f);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			hlog(f.cFileName);
		} 
		while(FindNextFile(h, &f));
	}
	else
	{
		hlog("Error opening directory");
	}

	return cPathInfo();
}

bool cFileSystem::copyFile(const string& source, const string& dest) const
{
	return CopyFile(source.c_str(), dest.c_str(), true);
}

bool cFileSystem::deleteFile(const string& file) const
{
	return DeleteFile(file.c_str());
}

bool cFileSystem::moveFile(const string& source, const string& dest) const
{
	return MoveFile(source.c_str(), dest.c_str());
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
	res.mFileType = cFileType::FT_FILE;

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

CLOSE_O2_NAMESPACE