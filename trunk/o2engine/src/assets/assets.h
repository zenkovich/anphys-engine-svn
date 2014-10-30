#ifndef ASSETS_H
#define ASSETS_H

#include "public.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class asAsset;

class Assets: public cSingleton<Assets>
{
public:
	asAsset* loadAsset(const string& path);
	bool saveAsset(const string& path);
};

CLOSE_O2_NAMESPACE

#endif // ASSETS_H
