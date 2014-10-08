#ifndef COLOR_H
#define COLOR_H

#include "util/public_namespace.h"
#include "math.h"

OPEN_O2_NAMESPACE
	
#define n255 0.00392156862745f

struct color4
{
	int r, g, b, a;

public:
	color4();
	color4(int cr, int cg, int cb, int ca);
	color4(float cr, float cg, float cb, float ca);

	color4 operator+(const color4& v) const;
	color4 operator+=(const color4& v);
	color4 operator-(const color4& v) const;
	color4 operator-=(const color4& v);

	color4 operator*(float v) const;
	color4 operator*=(float v);
	color4 operator/(float v) const;
	color4 operator/=(float v) ;
	color4 operator*(const color4& v) const;
	color4 operator/(const color4& v) const;
	color4 operator*=(const color4& v) const;

	bool operator==(const color4& color) const;
	
	float rf() const;
	float gf() const;
	float bf() const;
	float af() const;

	float getFloat(int i) const;
	char getParam(int i) const;

	void normalize();
	
	unsigned long ARGB() const;
	unsigned long ABGR() const;
	
	void setARGB(unsigned long color);	
	void setABGR(unsigned long color);

	unsigned long dword() const;
	void setDword(unsigned long color);

	static unsigned long dword(int r, int g, int b, int a);
	
	static color4 white();
	static color4 black();
	static color4 red();
	static color4 blue();
	static color4 green();
	static color4 gray();
	static color4 someColor(int idx);
};

//implemetation
color4::color4()
{
	r = g = b = a = 255;
}

color4::color4(int cr, int cg, int cb, int ca) 
{ 
	r = cr; g = cg; b = cb; a = ca;
}

color4::color4(float cr, float cg, float cb, float ca) 
{ 
	r = (int)(cr*255.0f);
	g = (int)(cg*255.0f);
	b = (int)(cb*255.0f);
	a = (int)(ca*255.0f);
}

color4 color4::operator+(const color4& v) const
{ 
	return color4(r + v.r, g + v.g, b + v.g, a + v.a);
}

color4 color4::operator+=(const color4& v) 
{
	*this = *this + v; 
	return *this;
}

color4 color4::operator-(const color4& v) const
{
	return color4(r - v.r, g - v.g, b - v.g, a - v.a);
}

color4 color4::operator-=(const color4& v) 
{ 
	*this = *this - v; 
	return *this; 
}

color4 color4::operator*(float v) const
{ 
	return color4((int)((float)r*v), (int)((float)g*v), (int)((float)b*v), (int)((float)a*v)); 
}

color4 color4::operator*=(float v)
{ 
	*this = (*this)*v; 
	return *this;
}

color4 color4::operator/(float v) const
{
	return *this*(1.0f/v);
}

color4 color4::operator/=(float v) 
{ 
	*this = *this/v; 
	return *this; 
}

color4 color4::operator*(const color4& v) const
{ 
	return color4(r*v.r/255, g*v.g/255, b*v.g/255, a*v.a/255);
}

color4 color4::operator/(const color4& v) const
{ 
	return color4( (int)( 255.0f*(rf()/v.rf()) ),
			       (int)( 255.0f*(gf()/v.gf()) ),
				   (int)( 255.0f*(bf()/v.bf()) ),
				   (int)( 255.0f*(af()/v.af()) ) );
}

color4 color4::operator*=(const color4& v) const
{
	return *this*v; 
}

bool color4::operator==(const color4& color) const
{
	return !(a != color.a || r != color.r || g != color.g || b != color.b);
}
	
float color4::rf() const
{
	return (float)r*n255;
}

float color4::gf() const
{
	return (float)g*n255; 
}

float color4::bf() const
{
	return (float)b*n255; 
}

float color4::af() const
{
	return (float)a*n255;
}

float color4::getFloat(int i) const
{
	return (float)(*(int*)(this + sizeof(float)*i))*n255;
	return 0;
}

char color4::getParam(int i) const
{	
	return (*(int*)(this + sizeof(float)*i));
	return 0;
}

void color4::normalize()
{
	r = clamp(r, 0, 255);
	g = clamp(g, 0, 255);
	b = clamp(b, 0, 255);
	a = clamp(a, 0, 255);
}
	
unsigned long color4::ARGB() const { return (unsigned long)( (a << 24) | (r << 16) | (g << 8) | b ); }
unsigned long color4::ABGR() const { return (unsigned long)( (a << 24) | (b << 16) | (g << 8) | r ); }
	
void color4::setARGB(unsigned long color) 
{ 
	a = (int)(color >> 24);
	r = (int)((color << 8) >> 24);
	g = (int)((color << 16) >> 24);
	b = (int)((color << 24) >> 24);
}
	
void color4::setABGR(unsigned long color) 
{ 
	a = (int)(color >> 24);
	b = (int)((color << 8) >> 24);
	g = (int)((color << 16) >> 24);
	r = (int)((color << 24) >> 24);
}

unsigned long color4::dword() const 
{
#ifdef RENDER_OGL
	return ABGR();
#elif defined(RENDER_D3D9C)
	return ARGB();
#endif
}

void color4::setDword(unsigned long color)  
{
#ifdef RENDER_OGL
	setABGR(color);
#elif defined(RENDER_D3D9C)
	setARGB(color);
#endif
}

unsigned long color4::dword(int r, int g, int b, int a)
{
#ifdef RENDER_OGL
	return (unsigned long)( (a << 24) | (b << 16) | (g << 8) | r );
#elif defined(RENDER_D3D9C)
	return (unsigned long)( (a << 24) | (r << 16) | (g << 8) | b );
#endif
}
	
color4 color4::white() { return color4(255, 255, 255, 255); }
color4 color4::black() { return color4(0, 0, 0, 255); }
color4 color4::red()   { return color4(255, 0, 0, 255); }
color4 color4::blue()  { return color4(0, 0, 255, 255); }
color4 color4::green() { return color4(0, 255, 0, 255); }
color4 color4::gray()  { return color4(150, 150, 150, 255); }
color4 color4::someColor(int idx) { return color4((idx*33)%255, (idx*58)%255, (idx*77)%255, 255); }

CLOSE_O2_NAMESPACE

#endif //COLOR_H