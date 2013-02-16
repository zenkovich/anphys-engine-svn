#ifndef LIGHT_BASE_INTERFACE_H
#define LIGHT_BASE_INTERFACE_H

#include "../util/math/mmath.h"
#include <string>
#include "../util/log/log_system.h"

struct grLightManager;

struct grLightBaseInterface
{
	enum grLightType{light_point_type, light_spot_type, light_directional_type};

	grLightManager* mLightManager;
	bool mActive;

	grLightBaseInterface(grLightManager* lightManager):mLightManager(lightManager), mActive(false) {}
	grLightBaseInterface():mLightManager(NULL), mActive(false) {}
	~grLightBaseInterface() {}

	virtual std::string getStructRealization() { return "base light"; }

	virtual void initialize(const grLightBaseInterface& light) { *gLog << formatStr("function initialize int %s not released\n", getStructRealization().c_str()); }
	virtual void initialize(grLightType type, color4 diffuse, 
		                    color4 specular, color4 ambient, vec3 position, 
							vec3 direction, float range, float fallOff, 
							float attenuation0, float attenuation1, float attenuation2, float theta, float phi)
		{ *gLog << formatStr("function initialize int %s not released\n", getStructRealization().c_str()); }

	virtual void setLightActive(bool active) { mActive = active; }
	virtual bool getLightActive() { return mActive; }

	virtual void setPos(vec3 position)
		{ *gLog << formatStr("function setPos int %s not released\n", getStructRealization().c_str()); }
	virtual vec3 getPos()
		{ *gLog << formatStr("function getPos int %s not released\n", getStructRealization().c_str()); return vec3(0); }

	virtual void setDirection(vec3 direction)
		{ *gLog << formatStr("function setDirection int %s not released\n", getStructRealization().c_str()); }
	virtual vec3 getDirection()
		{ *gLog << formatStr("function getDirection int %s not released\n", getStructRealization().c_str()); return vec3(0); }

	virtual grLightType getType()
		{ *gLog << formatStr("function getType int %s not released\n", getStructRealization().c_str()); return light_point_type; }

	virtual void setDiffuse(color4 diffuse)
		{ *gLog << formatStr("function setDiffuse int %s not released\n", getStructRealization().c_str()); }
	virtual color4 getDiffuse()
		{ *gLog << formatStr("function getDiffuse int %s not released\n", getStructRealization().c_str()); return color4(0); }

	virtual void setSpecular(color4 specular)
		{ *gLog << formatStr("function setSpecular int %s not released\n", getStructRealization().c_str()); }
	virtual color4 getSpecular()
		{ *gLog << formatStr("function getSpecular int %s not released\n", getStructRealization().c_str()); return color4(0); }

	virtual void setAmbient(color4 ambient)
		{ *gLog << formatStr("function setAmbient int %s not released\n", getStructRealization().c_str()); }
	virtual color4 getAmbient()
		{ *gLog << formatStr("function getAmbient int %s not released\n", getStructRealization().c_str()); return color4(0); }

	virtual void setRange(float range)
		{ *gLog << formatStr("function setRange int %s not released\n", getStructRealization().c_str()); }
	virtual float getRange()
		{ *gLog << formatStr("function getRange int %s not released\n", getStructRealization().c_str()); return 0; }

	virtual void setFallOff(float fallOff)
		{ *gLog << formatStr("function setFallOff int %s not released\n", getStructRealization().c_str()); }
	virtual float getFallOff()
		{ *gLog << formatStr("function getFallOff int %s not released\n", getStructRealization().c_str()); return 0; }

	virtual void setAttenuation(float attenuation, int idx)
		{ *gLog << formatStr("function setAttenuation int %s not released\n", getStructRealization().c_str()); }
	virtual float getAttenuation(int idx)
		{ *gLog << formatStr("function getAttenuation int %s not released\n", getStructRealization().c_str()); return 0; }

	virtual void setTheta(float theta)
		{ *gLog << formatStr("function setTheta int %s not released\n", getStructRealization().c_str()); }
	virtual float getTheta()
		{ *gLog << formatStr("function getTheta int %s not released\n", getStructRealization().c_str()); return 0; }

	virtual void setPhi(float phi)
		{ *gLog << formatStr("function setPhi int %s not released\n", getStructRealization().c_str()); }
	virtual float getPhi()
		{ *gLog << formatStr("function getPhi int %s not released\n", getStructRealization().c_str()); return 0; }
};

#endif //LIGHT_BASE_INTERFACE_H