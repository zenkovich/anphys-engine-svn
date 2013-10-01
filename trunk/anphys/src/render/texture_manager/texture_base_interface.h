#ifndef TEXTURE_BASE_INTERFACE_H
#define TEXTURE_BASE_INTERFACE_H

#include <string>
#include "util/math/mmath.h"
#include "util/log/log_system.h"

struct grTextureManager;

struct grTextureBaseInterface
{
	enum TextureUsage { TU_DEFAULT = 0, TU_RENDER_TEXTURE };

	grTextureManager* mTextureManager;

	TextureUsage      mUsage;
	vec2              mSize;
	bool              mLoaded;
	std::string       mFileName;

//functions
	grTextureBaseInterface(grTextureManager* textureManager = NULL):mTextureManager(textureManager), mLoaded(false), mUsage(TU_DEFAULT) {}
	virtual ~grTextureBaseInterface() {}

	virtual bool load(const std::string& fileName) {return false; }

	virtual void processStreaming(unsigned int maxStreamingData) {}

	virtual const char* getStructRealization() { return "base texture"; }
};

#endif //TEXTURE_BASE_INTERFACE_H