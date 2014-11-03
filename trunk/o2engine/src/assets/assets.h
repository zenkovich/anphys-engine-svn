#ifndef ASSETS_H
#define ASSETS_H

#include "public.h"
#include "util/singleton.h"
#include "util/file_system/file_info.h"

OPEN_O2_NAMESPACE

class asAsset;

class Assets: public cSingleton<Assets>
{
public:
	typedef array<cFileLocation> FilesLocsArr;

protected:
	FilesLocsArr mFilesLocations;

public:
	asAsset* loadAsset(const string& path);

	template<typename _asType>
	_asType* loadAsset(const string& path);

ENABLE_EDITING_ASSETS_PROTECTION	
	bool saveAsset(asAsset* asset, const string& path);
};

CLOSE_O2_NAMESPACE

#endif // ASSETS_H
