#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"
#include "../../engine/engine_options.h"

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

#ifdef RENDER_D3D8
#define D3DFVF_VERTEX_TEXTURE_LIGHT (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define D3DFVF_VERTEX_2D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#endif

struct vertexTexNorm/*: public vec3*/
{
	float x, y, z,
		  nx, ny, nz,
		  tu, tv;
	inline vertexTexNorm() { x = y = z = nx = ny = nz = tu = tv = 0; }
	inline vertexTexNorm(float px, float py, float pz) { x = px; y = py; z = pz; nx = ny = nz = tu = tv = 0; }
	inline vertexTexNorm(float px, float py, float pz, float pnx, float pny, float pnz, float ptu, float ptv) { x = px; y = py; z = pz; nx = pnx; ny = pny; nz = pnz; tu = ptu; tv = ptv; }
	inline vertexTexNorm(vec3 v) { x = v.x; y = v.y; z = v.z; nx = ny = nz = tu = tv = 0; }
	inline vertexTexNorm(vec3 v, vec3 n, float uu, float vv) { x = v.x; y = v.y; z = v.z; nx = n.x; ny = n.y; nz = n.z; tu = uu; tv = vv; }
	inline vec3 vt() { return vec3(x,y,z); }
		
};

inline void copyVertex(vertexTexNorm *in, vec3 *out, int count)
{
	for (int i = 0; i < count; i++)
		out[i] = vec3(in[i].x, in[i].y, in[i].z);
}

inline void copyVertex(int count, vertexTexNorm *out, vec3 *in)
{
	for (int i = 0; i < count; i++)
		out[i] = vertexTexNorm(in[i].x, in[i].y, in[i].z, 0, 1, 0, 0, 0);
}
inline void copyVertexRandomize(int count, vertexTexNorm *out, vec3 *in)
{
	for (int i = 0; i < count; i++)
		out[i] = vertexTexNorm(in[i].x, in[i].y, in[i].z, 0, 1, 0, (rand()%100)*0.01f, (rand()%100)*0.01f);
}

#ifdef PLATFORM_WIN32

struct vertex
{
	float x, y, z;
	DWORD color;
	inline vertex() {x = y = z = 0; color = 0; }
	inline vertex(float px, float py, float pz, DWORD pcolor) { x = px; y = py; z = pz; color = pcolor; }
};

struct vertex2d
{
	float x, y, z;
	DWORD color;
	float tu, tv;

	inline vertex2d() { x = y = z = tu = tv = 0;  }
	inline vertex2d(float px,float py,float pz,float ptu,float ptv, DWORD pcolor) { x = px; y = py; z = pz; color = pcolor; tu = ptu; tv = ptv;  }

};

#endif


#endif