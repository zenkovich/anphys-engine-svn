#ifndef LIGHT_H
#define LIGHT_H

#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/light/light_d3d8.h"
#endif //RENDER_D3D8

struct grLight:public grLightBase
{
	grLight(grLightManager* lightManager):grLightBase(lightManager) {}
	grLight():grLightBase() {}
};

#endif //LIGHT_H