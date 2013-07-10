#ifndef VERTEX_H
#define VERTEX_H

/** Vertex for d3d8 render. Containg 3D coordinates, color, texture coordinates. */
struct vertex2
{
	float         x, y, z;
	unsigned long color;
	float         tu, tv;

	inline vertex2():x(0), y(0), z(0), tu(0), tv(0), color(0) {}

	inline vertex2(float px,float py,float pz,float ptu,float ptv, unsigned long pcolor):
		x(px), y(py), z(pz), color(pcolor), tu(ptu), tv(ptv) {}

};

#endif //VERTEX_H