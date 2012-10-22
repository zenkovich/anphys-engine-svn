#include "../../engine/engine_options.h"

#ifdef RENDER_D3D8
#include "light_d3d8.h"
#include "../render.h"
#include "../light_manager.h"

grLightBase::grLightBase(grLightManager* lightManager):grLightBaseInterface(lightManager), mIdx(0) {}
grLightBase::grLightBase():grLightBaseInterface(), mIdx(0) {}
grLightBase::~grLightBase()
{
	grLightBaseInterface::~grLightBaseInterface();
}

void grLightBase::initialize(const grLightBaseInterface& light)
{
}

void grLightBase::initialize(grLightType type, color4 diffuse, 
		                    color4 specular, color4 ambient, vec3 position, 
							vec3 direction, float range, float fallOff, 
							float attenuation0, float attenuation1, float attenuation2, float theta, float phi)
{	
	mActive = false;
	
	mLight.Ambient = D3DXCOLOR(ambient.rf(), ambient.gf(), ambient.bf(), ambient.af());
	mLight.Diffuse = D3DXCOLOR(diffuse.rf(), diffuse.gf(), diffuse.bf(), diffuse.af());
	mLight.Specular = D3DXCOLOR(specular.rf(), specular.gf(), specular.bf(), specular.af());
	
	mLight.Attenuation0 = attenuation0;
	mLight.Attenuation1 = attenuation1;
	mLight.Attenuation2 = attenuation2;

	mLight.Direction = D3DXVECTOR3(direction.x, direction.y, direction.z);
	mLight.Falloff = fallOff;
	mLight.Phi = phi;
	mLight.Position = D3DXVECTOR3(position.x, position.y, position.z);
	mLight.Range = range;
	mLight.Theta = theta;
	if (type = light_point_type) mLight.Type = D3DLIGHT_POINT;
	if (type = light_spot_type) mLight.Type = D3DLIGHT_SPOT;
	if (type = light_directional_type) mLight.Type = D3DLIGHT_DIRECTIONAL;
}
	
void grLightBase::setLightActive(bool active)
{
	mActive = active;

	mLightManager->mRender->m_pDirect3DDevice->LightEnable(mIdx, active);
}

void grLightBase::setPos(vec3 position)
{
	mLight.Position = D3DXVECTOR3(position.x, position.y, position.z);
}

vec3 grLightBase::getPos()
{
	return vec3(mLight.Position.x, mLight.Position.y, mLight.Position.z);
}

void grLightBase::setDirection(vec3 direction)
{
	mLight.Direction = D3DXVECTOR3(direction.x, direction.y, direction.z);
}

vec3 grLightBase::getDirection()
{
	return vec3(mLight.Direction.x, mLight.Direction.y, mLight.Direction.z);
}

grLightBaseInterface::grLightType grLightBase::getType()
{
	if (mLight.Type = D3DLIGHT_POINT) return grLightBaseInterface::light_point_type;
	if (mLight.Type = D3DLIGHT_SPOT) return grLightBaseInterface::light_spot_type;
	if (mLight.Type = D3DLIGHT_DIRECTIONAL) return grLightBaseInterface::light_directional_type;
	return grLightBaseInterface::light_spot_type;
}

void grLightBase::setDiffuse(color4 diffuse)
{
	mLight.Diffuse = D3DXCOLOR(diffuse.rf(), diffuse.gf(), diffuse.bf(), diffuse.af());
}

color4 grLightBase::getDiffuse()
{
	return color4(mLight.Diffuse.r, mLight.Diffuse.g, mLight.Diffuse.b, mLight.Diffuse.a);
}

void grLightBase::setSpecular(color4 specular)
{
	mLight.Specular = D3DXCOLOR(specular.rf(), specular.gf(), specular.bf(), specular.af());
}

color4 grLightBase::getSpecular()
{
	return color4(mLight.Specular.r, mLight.Specular.g, mLight.Specular.b, mLight.Specular.a);
}

void grLightBase::setAmbient(color4 ambient)
{
	mLight.Ambient = D3DXCOLOR(ambient.rf(), ambient.gf(), ambient.bf(), ambient.af());
}

color4 grLightBase::getAmbient()
{
	return color4(mLight.Ambient.r, mLight.Ambient.g, mLight.Ambient.b, mLight.Ambient.a);
}

void grLightBase::setRange(float range)
{
	mLight.Range = range;
}

float grLightBase::getRange()
{
	return mLight.Range;
}

void grLightBase::setFallOff(float fallOff)
{
	mLight.Falloff = fallOff;
}

float grLightBase::getFallOff()
{
	return mLight.Falloff;
}

void grLightBase::setAttenuation(float attenuation, int idx)
{
	if (idx = 0) mLight.Attenuation0 = attenuation;
	if (idx = 1) mLight.Attenuation1 = attenuation;
	if (idx = 2) mLight.Attenuation2 = attenuation;
}

float grLightBase::getAttenuation(int idx)
{
	if (idx == 0) return mLight.Attenuation0;
	if (idx == 1) return mLight.Attenuation1;
	if (idx == 2) return mLight.Attenuation2;
	return 0;
}

void grLightBase::setTheta(float theta)
{
	mLight.Theta = theta;
}

float grLightBase::getTheta()
{
	return mLight.Theta;
}

void grLightBase::setPhi(float phi)
{
	mLight.Phi = phi;
}

float grLightBase::getPhi()
{
	return mLight.Phi;
}


#endif //RENDER_D3D8