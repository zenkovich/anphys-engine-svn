#ifndef COLOR_H
#define COLOR_H

#include <math.h>

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
		r = (int)(cr*255.0f);
		g = (int)(cg*255.0f);
		b = (int)(cb*255.0f);
		a = (int)(ca*255.0f);
	}

	inline color4 operator+(const color4& v) const
	{ 
		return color4(r + v.r, g + v.g, b + v.g, a + v.a);
	}

	inline color4 operator+=(const color4& v) 
	{
		*this = *this + v; 
		return *this;
	}

	inline color4 operator-(const color4& v) const
	{
		return color4(r - v.r, g - v.g, b - v.g, a - v.a);
	}

	inline color4 operator-=(const color4& v) 
	{ 
		*this = *this - v; 
		return *this; 
	}

	inline color4 operator*(float v) const
	{ 
		return color4((int)((float)r*v), (int)((float)g*v), (int)((float)b*v), (int)((float)a*v)); 
	}

	inline color4 operator*=(float v)
	{ 
		*this = (*this)*v; 
		return *this;
	}

	inline color4 operator/(float v) const
	{
		return *this*(1.0f/v);
	}

	inline color4 operator/=(float v) 
	{ 
		*this = *this/v; 
		return *this; 
	}

	inline color4 operator*(const color4& v) const
	{ 
		return color4(r*v.r, g*v.g, b*v.g, a*v.a);
	}

	inline color4 operator/(const color4& v) const
	{ 
		return color4(r/v.r, g/v.g, b/v.g, a/v.a);
	}

	inline color4 operator*=(const color4& v) const
	{
		return *this*v; 
	}

	inline bool operator==(const color4& color)
	{
		if (abs(r - color.r) > 0 || abs(g - color.g) > 0 || abs(b - color.b) > 0 || abs(a - color.a) > 0)
			return false;
		return true;
	}
	
	inline float rf() const
	{
		return (float)r*n255;
	}

	inline float gf() const
	{
		return (float)g*n255; 
	}

	inline float bf() const
	{
		return (float)b*n255; 
	}

	inline float af() const
	{
		return (float)a*n255;
	}

	inline float getFloat(int i) const
	{
		/*if (i == 0) return (float)r*n255;
		if (i == 1) return (float)g*n255;
		if (i == 2) return (float)b*n255;
		if (i == 3) return (float)a*n255;*/
		return (float)(*(int*)(this + sizeof(float)*i))*n255;
		return 0;
	}

	inline char getParam(int i) const
	{
		/*if (i == 0) return r;
		if (i == 1) return g;
		if (i == 2) return b;
		if (i == 3) return a;*/		
		return (*(int*)(this + sizeof(float)*i));
		return 0;
	}

	inline void normalize()
	{
		r = std::max<int>(std::min<int>(255, r), 0);
		g = std::max<int>(std::min<int>(255, g), 0);
		b = std::max<int>(std::min<int>(255, b), 0);
		a = std::max<int>(std::min<int>(255, a), 0);
	}

	unsigned long dwordARGB() const { return (unsigned long)( (a << 24) | (r << 16) | (g << 8) | b ); }
	
	void setDwordARGB(unsigned long color) 
	{ 
		a = (int)(color >> 24);
		r = (int)((color << 8) >> 24);
		g = (int)((color << 16) >> 24);
		b = (int)((color << 24) >> 24);
	}

};

#endif