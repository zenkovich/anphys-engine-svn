#ifndef LIGHT_D3D8_H
#define LIGHT_D3D8_H

#include "../light_base_interface.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <d3dx8core.h>

struct grLightBase:public grLightBaseInterface
{	
	D3DLIGHT8 mLight;
	int mIdx;

	grLightBase(grLightManager* lightManager);
	grLightBase();
	~grLightBase();

	std::string getStructRealization() { return "lightD3D8"; }

	void initialize(const grLightBaseInterface& light);
	void initialize(grLightType type, color4 diffuse, 
		                    color4 specular, color4 ambient, vec3 position, 
							vec3 direction, float range, float fallOff, 
							float attenuation0, float attenuation1, float attenuation2, float theta, float phi);

	void idx(int idx) { mIdx = idx; }

	void setLightActive(bool active);

	void setPos(vec3 position);
	vec3 getPos();

	void setDirection(vec3 direction);
	vec3 getDirection();

	grLightBaseInterface::grLightType getType();

	void setDiffuse(color4 diffuse);
	color4 getDiffuse();

	void setSpecular(color4 specular);
	color4 getSpecular();

	void setAmbient(color4 ambient);
	color4 getAmbient();

	void setRange(float range);
	float getRange();

	void setFallOff(float fallOff);
	float getFallOff();

	void setAttenuation(float attenuation, int idx);
	float getAttenuation(int idx);

	void setTheta(float theta);
	float getTheta();

	void setPhi(float phi);
	float getPhi();
};

#endif //LIGHT_D3D8_H