#ifndef LIGHT_BASE_INTERFACE_H
#define LIGHT_BASE_INTERFACE_H

#include <string>
#include "util/math/mmath.h"
#include "util/log/log_system.h"

struct grLightManager;

struct grLightBaseInterface
{
	enum grLightType{light_point_type, light_spot_type, light_directional_type};

	grLightManager* mLightManager;
	bool            mActive;

//functions
	grLightBaseInterface(grLightManager* lightManager):mLightManager(lightManager), mActive(false) {}
	grLightBaseInterface():mLightManager(NULL), mActive(false) {}
	virtual ~grLightBaseInterface() {}

	virtual const char* getStructRealization() { return "base light"; }

	virtual void initialize(const grLightBaseInterface& light) {}
	virtual void initialize(grLightType type, color4 diffuse, 
		                    color4 specular, color4 ambient, vec3 position, 
							vec3 direction, float range, float fallOff, 
							float attenuation0, float attenuation1, float attenuation2, float theta, float phi) {}

	virtual void setLightActive(bool active) { mActive = active; }
	virtual bool getLightActive() { return mActive; }

	virtual void setPos(vec3 position) {}
	virtual vec3 getPos() { return vec3(0); }

	virtual void setDirection(vec3 direction) {}
	virtual vec3 getDirection() {return vec3(0); }

	virtual grLightType getType() { return light_point_type; }

	virtual void setDiffuse(color4 diffuse) {}
	virtual color4 getDiffuse() { return color4(0); }

	virtual void setSpecular(color4 specular) {}
	virtual color4 getSpecular() { return color4(0); }

	virtual void setAmbient(color4 ambient) {}
	virtual color4 getAmbient() { return color4(0); }

	virtual void setRange(float range) {}
	virtual float getRange() { return 0; }

	virtual void setFallOff(float fallOff) {}
	virtual float getFallOff() { return 0; }

	virtual void setAttenuation(float attenuation, int idx) {}
	virtual float getAttenuation(int idx) { return 0; }

	virtual void setTheta(float theta) {}
	virtual float getTheta() { return 0; }

	virtual void setPhi(float phi) {}
	virtual float getPhi() { return 0; }
};

#endif //LIGHT_BASE_INTERFACE_H