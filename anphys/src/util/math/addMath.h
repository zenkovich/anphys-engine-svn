#ifndef ADD_MATH_H
#define ADD_MATH_H

#include "vector.h"

//забить вершины бокса в массив
inline void CubeXYZ(vec3 vx, vec3 vy, vec3 vz, vec3 *out)
{
	out[0] = vx*-1.0f - vy - vz;
	out[1] = vx*-1.0f + vy - vz;
	out[2] = vx + vy - vz;
	out[3] = vx - vy - vz;
	out[4] = vx*-1.0f - vy + vz;
	out[5] = vx*-1.0f + vy + vz;
	out[6] = vx + vy + vz;
	out[7] = vx - vy + vz;
}

#endif