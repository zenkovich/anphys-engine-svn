#ifndef VERTEX_H
#define VERTEX_H

#include "util/public_namespace.h"
#include "vector2.h"

OPEN_O2_NAMESPACE

struct vertex2:public vec2<float>
{
//  float         x, y; - from vec2<float>
	float         z;
	unsigned long color;
	float         tu, tv;

public:
	vertex2();
	vertex2(float vx, float vy);
	vertex2(float vx, float vy, float vz);
	vertex2(float vx, float vy, float vz, unsigned long vcolor, float vtu, float vtv);
	vertex2(float vx, float vy, unsigned long vcolor, float vtu, float vtv);
	vertex2(const vec2f& pos, unsigned long vcolor, float vtu, float vtv);
		
	void set(const vec2f& pos, unsigned long ccolor, float u, float v);		
	void set(float px, float py, unsigned long ccolor, float u, float v);
	void set(const vec2f& pos, float cz, unsigned long ccolor, float u, float v);
};

//implementation
vertex2::vertex2():vec2<float>(), color(0), tu(0), tv(0) {}

vertex2::vertex2(float vx, float vy):vec2<float>(vx, vy), z(1), color(0), tu(0), tv(0) {}

vertex2::vertex2(float vx, float vy, float vz):vec2<float>(vx, vy), z(vz), color(0), tu(0), tv(0) {}

vertex2::vertex2(float vx, float vy, float vz, unsigned long vcolor, float vtu, float vtv):
	vec2<float>(vx, vy), z(vz), color(vcolor), tu(vtu), tv(vtv) {}

vertex2::vertex2(float vx, float vy, unsigned long vcolor, float vtu, float vtv):
	vec2<float>(vx, vy), z(1), color(vcolor), tu(vtu), tv(vtv) {}

vertex2::vertex2(const vec2f& pos, unsigned long vcolor, float vtu, float vtv):
	vec2<float>(pos), z(1), color(vcolor), tu(vtu), tv(vtv) {}
		
void vertex2::set(const vec2f& pos, unsigned long ccolor, float u, float v) 
{
	x = pos.x; y = pos.y;
	color = ccolor;
	tu = u; tv = v;
}
		
void vertex2::set(float px, float py, unsigned long ccolor, float u, float v) 
{
	x = px; y = py;
	color = ccolor;
	tu = u; tv = v;
}

void vertex2::set(const vec2f& pos, float cz, unsigned long ccolor, float u, float v)
{
	x = pos.x; y = pos.y;
	z = cz;
	color = ccolor;
	tu = u; tv = v;
}

CLOSE_O2_NAMESPACE

#endif //VERTEX_H