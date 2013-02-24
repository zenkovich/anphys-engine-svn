#ifndef COLOR_H
#define COLOR_H

#include "../../engine/engine_options.h"
#ifdef RENDER_DIRECT3D9
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>
#endif*/

#define n255 0.00392156862745f

struct color4
{
	int r, g, b, a;

	inline color4() { r = g = b = a = 0; }
	inline color4(int cr, int cg, int cb, int ca) { r = cr; g = cg; b = cb; a = ca; }
	inline color4(int c) { r = g = b = a = c; }
	inline color4(float cr, float cg, float cb, float ca) 
	{ 
		r = (int)(cr*255);
		g = (int)(cg*255);
		b = (int)(cb*255);
		a = (int)(ca*255);
	}
	
	inline float getRfloat() { return r*n255; }
	inline float getGfloat() { return g*n255; }
	inline float getBfloat() { return b*n255; }
	inline float getFfloat() { return a*n255; }

	inline float getFloat(int i) 
	{
		if (i == 0) return r*n255;
		if (i == 1) return g*n255;
		if (i == 2) return b*n255;
		if (i == 3) return a*n255;
		return 0;
	}

	inline char getParam(int i)
	{
		if (i == 0) return r;
		if (i == 1) return g;
		if (i == 2) return b;
		if (i == 3) return a;
		return 0;
	}
	
	inline float rf() { return r*n255; }
	inline float gf() { return g*n255; }
	inline float bf() { return b*n255; }
	inline float af() { return a*n255; }

#ifdef RENDER_DIRECT3D8
	inline void operator=(D3DCOLORVALUE cold3d)
	{
		*this = color4(cold3d.r, cold3d.g, cold3d.b, cold3d.a);
	}
#endif

};

#ifdef RENDER_DIRECT3D8

inline D3DCOLORVALUE color4f2D3Dcolor(color4 colr)
{
	D3DCOLORVALUE val;

	val.r = colr.rf();
	val.g = colr.gf();
	val.b = colr.bf();
	val.a = colr.af();

	return val;
}
#endif

#endif