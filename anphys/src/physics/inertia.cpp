#include "inertia.h"

mat3x3 getBoxInertia( float mass, const vec3& size )
{
	float koef=2.0f*mass*0.1f;

	return mat3x3((size.y*size.y+size.z*size.z)*koef,0,0,
		          0,(size.x*size.x+size.z*size.z)*koef,0,
				  0,0,(size.y*size.y+size.x*size.x)*koef);
}
