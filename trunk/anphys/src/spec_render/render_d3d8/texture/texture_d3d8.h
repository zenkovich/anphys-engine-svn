#ifndef TEXTURE_D3D8_H
#define TEXTURE_D3D8_H

#include "render/texture_manager/texture_base_interface.h"
#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

struct grTextureBase:public grTextureBaseInterface
{
	LPDIRECT3DTEXTURE8 mTexturePtr;

	grTextureBase(grTextureManager* textureManager);
	~grTextureBase();

	bool load(const std::string& fileName);

	const char* getStructRealization() { return "textureD3D8"; }
};

#endif //TEXTURE_D3D8_H