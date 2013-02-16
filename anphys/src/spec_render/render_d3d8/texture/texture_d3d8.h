#ifndef TEXTURE_D3D8_H
#define TEXTURE_D3D8_H

#include "../texture_base_interface.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <d3dx8core.h>

struct grTextureBase:public grTextureBaseInterface
{
	LPDIRECT3DTEXTURE8 mTexturePtr;

	grTextureBase(grTextureManager* textureManager);
	~grTextureBase();

	bool load(const std::string& fileName);

	std::string getStructRealization() { return "textureD3D8"; }
};

#endif //TEXTURE_D3D8_H