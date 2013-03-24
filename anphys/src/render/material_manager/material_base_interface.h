#ifndef MATERIAL_BASE_INTRFACE_H
#define MATERIAL_BASE_INTRFACE_H


#include <string>
#include "util/math/mmath.h"
#include "util/log/log_system.h"

struct grMaterialManager;

struct grMaterialBaseInterface
{
	grMaterialManager* mMaterialManager;

//functions
	grMaterialBaseInterface(grMaterialManager* materialManager):mMaterialManager(materialManager) {  }
	grMaterialBaseInterface(grMaterialManager* materialManager, const color4& ambient, 
		                    const color4& diffuse, const color4& emissive, const color4& specular,
		                    float power):mMaterialManager(materialManager) 
	{ 
		initialize(ambient, diffuse, emissive, specular, power);
	}

	virtual ~grMaterialBaseInterface() {}

	virtual void initialize(const color4& ambient, const color4& diffuse, const color4& emissive, const color4& specular,
		            float power)
	{
		setAmbient(ambient); setDiffuse(diffuse); setEmissive(emissive); setSpecular(specular); setPower(power);
	}

	virtual void setAmbient(color4 ambient) {}
	virtual void setDiffuse(color4 diffuse) {}
	virtual void setEmissive(color4 emissive) {}
	virtual void setSpecular(color4 specular) {}
	virtual void setPower(float power) {}

	virtual color4 getAmbient() { return color4(0); }
	virtual color4 getDiffuse() { return color4(0); }
	virtual color4 getEmissive() {return color4(0); }
	virtual color4 getSpecular() { return color4(0); }
	virtual float getPower() { return 0; }

	virtual const char* getStructRealization() { return "base material"; }
};


#endif //MATERIAL_BASE_INTRFACE_H