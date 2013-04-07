#ifndef COLOR_H
#define COLOR_H

#include "../../engine/engine_options.h"

#define n255 0.00392156862745f

struct color4
{
	int r, g, b, a;

	inline color4()
	{
		r = g = b = a = 0;
	}

	inline color4(int cr, int cg, int cb, int ca) 
	{ 
		r = cr; g = cg; b = cb; a = ca;
	}

	inline color4(int c) 
	{ 
		r = g = b = a = c;
	}

	inline color4(float cr, float cg, float cb, float ca) 
	{ 
		r = (int)(cr*255);
		g = (int)(cg*255);
		b = (int)(cb*255);
		a = (int)(ca*255);
	}
	
	inline float rf() const
	{
		return r*n255;
	}

	inline float gf() const
	{
		return g*n255; 
	}

	inline float bf() const
	{
		return b*n255; 
	}

	inline float af() const
	{
		return a*n255;
	}

	inline float getFloat(int i) const
	{
		if (i == 0) return r*n255;
		if (i == 1) return g*n255;
		if (i == 2) return b*n255;
		if (i == 3) return a*n255;
		return 0;
	}

	inline char getParam(int i) const
	{
		if (i == 0) return r;
		if (i == 1) return g;
		if (i == 2) return b;
		if (i == 3) return a;
		return 0;
	}

	unsigned long dwordARGB() const { return (unsigned long)( (a << 24) | (r << 16) | (g << 8) | b ); }
	
	void setDwordARGB(unsigned long color) 
	{ 
		a = (int)(color >> 24);
		r = (int)((color << 8) >> 24);
		g = (int)((color << 16) >> 24);
		b = (int)((color << 24) >> 24);
	}

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