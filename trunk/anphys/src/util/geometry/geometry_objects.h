#ifndef GEMETRY_OBJECTS_H
#define GEMETRY_OBJECTS_H

#include "../utils.h"

void boxGeometry(vertexTexNorm* verticies, int &vertCount, int* indexes, int &polyCount, vec3 size);
void arrowGeometry(vertexTexNorm* verticies, int &vertCount, int* indexes, int &polyCount, vec3 size);

#endif //GEMETRY_OBJECTS_H