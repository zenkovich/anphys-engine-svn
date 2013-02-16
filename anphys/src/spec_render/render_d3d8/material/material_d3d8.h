#ifndef MATERIAL_D3D8_H
#define MATERIAL_D3D8_H


#include "../material_base_interface.h"
#include "../../util/memory/mem_utils.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <d3dx8core.h>

struct grMaterialBase:public grMaterialBaseInterface
{
	D3DMATERIAL8* mMaterial;

	grMaterialBase(grMaterialManager* materialManager):grMaterialBaseInterface(materialManager) 
	{ 
		mMaterial = new D3DMATERIAL8;
	}

	grMaterialBase(grMaterialManager* materialManager, const color4& ambient, 
		                    const color4& diffuse, const color4& emissive, const color4& specular,
		                    float power):grMaterialBaseInterface(materialManager) 
	{ 
		mMaterial = new D3DMATERIAL8;
		initialize(ambient, diffuse, emissive, specular, power);
	}

	~grMaterialBase()
	{
		safe_release(mMaterial);
	}

	void setAmbient(color4 ambient) { mMaterial->Ambient = D3DXCOLOR(ambient.rf(), ambient.gf(), ambient.bf(), ambient.af()); }
	void setDiffuse(color4 diffuse) { mMaterial->Diffuse = D3DXCOLOR(diffuse.rf(), diffuse.gf(), diffuse.bf(), diffuse.af()); }
	void setEmissive(color4 emissive) { mMaterial->Emissive = D3DXCOLOR(emissive.rf(), emissive.gf(), emissive.bf(), emissive.af()); }
	void setSpecular(color4 specular) { mMaterial->Specular = D3DXCOLOR(specular.rf(), specular.gf(), specular.bf(), specular.af()); }
	void setPower(float power) { mMaterial->Power = power; }

	color4 getAmbient() const { return color4(mMaterial->Ambient.r, mMaterial->Ambient.g, mMaterial->Ambient.b, mMaterial->Ambient.a); }
	color4 getDiffuse() const { return color4(mMaterial->Diffuse.r, mMaterial->Diffuse.g, mMaterial->Diffuse.b, mMaterial->Diffuse.a); }
	color4 getEmissive() const { return color4(mMaterial->Emissive.r, mMaterial->Emissive.g, mMaterial->Emissive.b, mMaterial->Emissive.a); }
	color4 getSpecular() const { return color4(mMaterial->Specular.r, mMaterial->Specular.g, mMaterial->Specular.b, mMaterial->Specular.a); }
	float getPower() const { return mMaterial->Power; }

	std::string getStructRealization() { return "materiald3d8"; }

};

#endif //MATERIAL_D3D8_H