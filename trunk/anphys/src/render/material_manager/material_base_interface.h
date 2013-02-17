#ifndef MATERIAL_BASE_INTRFACE_H
#define MATERIAL_BASE_INTRFACE_H


#include <string>
#include "util/math/mmath.h"
#include "util/log/log_system.h"

struct grMaterialManager;

struct grMaterialBaseInterface
{
	grMaterialManager* mMaterialManager;

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

	virtual void setAmbient(color4 ambient) { *gLog << formatStr("function setAmbient in %s not released\n", getStructRealization().c_str()); }
	virtual void setDiffuse(color4 diffuse) { *gLog << formatStr("function setDiffuse in %s not released\n", getStructRealization().c_str()); }
	virtual void setEmissive(color4 emissive) { *gLog << formatStr("function setEmissive in %s not released\n", getStructRealization().c_str()); }
	virtual void setSpecular(color4 specular) { *gLog << formatStr("function setSpecular in %s not released\n", getStructRealization().c_str()); }
	virtual void setPower(float power) { *gLog << formatStr("function setPower in %s not released\n", getStructRealization().c_str()); }

	virtual color4 getAmbient() { *gLog << formatStr("function getAmbient in %s not released\n", getStructRealization().c_str()); return color4(); }
	virtual color4 getDiffuse() { *gLog << formatStr("function getAmgetDiffusebient in %s not released\n", getStructRealization().c_str()); return color4(); }
	virtual color4 getEmissive() { *gLog << formatStr("function getEmissive in %s not released\n", getStructRealization().c_str()); return color4(); }
	virtual color4 getSpecular() { *gLog << formatStr("function getSpecular in %s not released\n", getStructRealization().c_str()); return color4(); }
	virtual float getPower() { *gLog << formatStr("function getPower in %s not released\n", getStructRealization().c_str()); return 0; }

	virtual std::string getStructRealization() { return "base material"; }
};


#endif //MATERIAL_BASE_INTRFACE_H