#ifndef TEXTURE_BASE_INTERFACE_H
#define TEXTURE_BASE_INTERFACE_H

#include <string>
#include "util/math/mmath.h"
#include "util/log/log_system.h"

struct grTextureManager;

struct grTextureBaseInterface
{
	grTextureManager* mTextureManager;

	vec2              mSize;
	bool              mLoaded;
	std::string       mFileName;

//functions
	grTextureBaseInterface(grTextureManager* textureManager = NULL):mTextureManager(textureManager), mLoaded(false) {}
	virtual ~grTextureBaseInterface() {}

	virtual bool load(const std::string& fileName) 
	{ 
		*gLog << formatStr("function load() in %s not released\n", getStructRealization()); return false; 
	}

	virtual void processStreaming(unsigned int maxStreamingData) {}

	virtual const char* getStructRealization() { return "base texture"; }
};

#endif //TEXTURE_BASE_INTERFACE_H