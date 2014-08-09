#include "file_system.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cFileSystem);

cFileSystem::cFileSystem()
{
	mExtensions[cFileType::FT_IMAGE].push_back("png");

	mExtensions[cFileType::FT_CONFIG].push_back("xml");
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
	return cPathInfo();
}

bool cFileSystem::copyFile(const string& source, const string& dest) const
{
	return false;
}

bool cFileSystem::deleteFile(const string& file) const
{
	return false;
}

bool cFileSystem::moveFile(const string& source, const string& dest) const
{
	if (!copyFile(source, dest))
		return false;

	return deleteFile(source);
}

CLOSE_O2_NAMESPACE