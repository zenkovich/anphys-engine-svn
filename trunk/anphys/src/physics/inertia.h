#ifndef INERTIA_H
#define INERTIA_H

#include "../util/math/mmath.h"

mat3x3 getBoxInertia(float mass, const vec3& size);

#endif //INERTIA_H