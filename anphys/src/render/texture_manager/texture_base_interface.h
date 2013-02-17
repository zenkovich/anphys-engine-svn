#ifndef TEXTURE_BASE_INTERFACE_H
#define TEXTURE_BASE_INTERFACE_H

#include <string>
#include "util/math/mmath.h"
#include "util/log/log_system.h"

struct grTextureManager;

struct grTextureBaseInterface
{
	grTextureManager* mTextureManager;

	vec2 mSize;
	bool mLoaded;
	std::string mFileName;

	grTextureBaseInterface(grTextureManager* textureManager = NULL):mTextureManager(textureManager), mLoaded(false) {  }
	virtual ~grTextureBaseInterface() {}

	virtual bool load(const std::string& fileName) { *gLog << formatStr("function load() in %s not released\n", getStructRealization().c_str()); return false; }

	virtual std::string getStructRealization() { return "base texture"; }
};

#endif //TEXTURE_BASE_INTERFACE_H