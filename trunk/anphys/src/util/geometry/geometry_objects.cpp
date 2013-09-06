#include "geometry_objects.h"



void boxGeometry(vertexTexNorm* verticies, int &vertCount, int* indexes, int &polyCount, vec3 size)
{
	polyCount = 0;
	vec3 hs = size*0.5f;
	verticies[0] = vertexTexNorm(-hs.x, -hs.y, -hs.z, 0, 0, -1, 0, 0);
	verticies[1] = vertexTexNorm( hs.x, -hs.y, -hs.z, 0, 0, -1, 0, 1);
	verticies[2] = vertexTexNorm( hs.x,  hs.y, -hs.z, 0, 0, -1, 1, 1);
	verticies[3] = vertexTexNorm(-hs.x,  hs.y, -hs.z, 0, 0, -1, 1, 0);
	indexes[polyCount++] = 2; indexes[polyCount++] = 1; indexes[polyCount++] = 0;
	indexes[polyCount++] = 3; indexes[polyCount++] = 2; indexes[polyCount++] = 0;
	
	verticies[4] = vertexTexNorm(-hs.x, -hs.y, hs.z, 0, 0, 1, 0, 0);
	verticies[5] = vertexTexNorm( hs.x, -hs.y, hs.z, 0, 0, 1, 0, 1);
	verticies[6] = vertexTexNorm( hs.x,  hs.y, hs.z, 0, 0, 1, 1, 1);
	verticies[7] = vertexTexNorm(-hs.x,  hs.y, hs.z, 0, 0, 1, 1, 0);
	indexes[polyCount++] = 6; indexes[polyCount++] = 4; indexes[polyCount++] = 5;
	indexes[polyCount++] = 7; indexes[polyCount++] = 4; indexes[polyCount++] = 6;
	
	verticies[8] = vertexTexNorm(-hs.x, -hs.y, -hs.z, -1, 0, 0, 0, 0);
	verticies[9] = vertexTexNorm(-hs.x,  hs.y, -hs.z, -1, 0, 0, 0, 1);
	verticies[10] = vertexTexNorm(-hs.x,  hs.y,  hs.z, -1, 0, 0, 1, 1);
	verticies[11] = vertexTexNorm(-hs.x, -hs.y,  hs.z, -1, 0, 0, 1, 0);
	indexes[polyCount++] = 8; indexes[polyCount++] = 10; indexes[polyCount++] = 9;
	indexes[polyCount++] = 8; indexes[polyCount++] = 11; indexes[polyCount++] = 10;
	
	verticies[12] = vertexTexNorm(hs.x, -hs.y, -hs.z, 1, 0, 0, 0, 0);
	verticies[13] = vertexTexNorm(hs.x,  hs.y, -hs.z, 1, 0, 0, 0, 1);
	verticies[14] = vertexTexNorm(hs.x,  hs.y,  hs.z, 1, 0, 0, 1, 1);
	verticies[15] = vertexTexNorm(hs.x, -hs.y,  hs.z, 1, 0, 0, 1, 0);
	indexes[polyCount++] = 13; indexes[polyCount++] = 14; indexes[polyCount++] = 12;
	indexes[polyCount++] = 14; indexes[polyCount++] = 15; indexes[polyCount++] = 12;
	
	verticies[16] = vertexTexNorm(-hs.x, -hs.y, -hs.z, 0, -1, 0, 0, 0);
	verticies[17] = vertexTexNorm( hs.x, -hs.y, -hs.z, 0, -1, 0, 0, 1);
	verticies[18] = vertexTexNorm( hs.x, -hs.y,  hs.z, 0, -1, 0, 1, 1);
	verticies[19] = vertexTexNorm(-hs.x, -hs.y,  hs.z, 0, -1, 0, 1, 0);
	indexes[polyCount++] = 17; indexes[polyCount++] = 18; indexes[polyCount++] = 16;
	indexes[polyCount++] = 18; indexes[polyCount++] = 19; indexes[polyCount++] = 16;
	
	verticies[20] = vertexTexNorm(-hs.x, hs.y, -hs.z, 0, 1, 0, 0, 0);
	verticies[21] = vertexTexNorm( hs.x, hs.y, -hs.z, 0, 1, 0, 0, 1);
	verticies[22] = vertexTexNorm( hs.x, hs.y,  hs.z, 0, 1, 0, 1, 1);
	verticies[23] = vertexTexNorm(-hs.x, hs.y,  hs.z, 0, 1, 0, 1, 0);
	indexes[polyCount++] = 20; indexes[polyCount++] = 22; indexes[polyCount++] = 21;
	indexes[polyCount++] = 20; indexes[polyCount++] = 23; indexes[polyCount++] = 22;

	polyCount /= 3;
	vertCount = 24;
}

void arrowGeometry(vertexTexNorm* verticies, int &vertCount, int* indexes, int &polyCount, vec3 size)
{
	float bx = size.x*0.1f;
	float by = size.y*0.9f;
	float bz = size.z*0.1f;
	
	verticies[0]=vertexTexNorm(-bx, 0 , -bz,  0, -1 ,0,  0, 1);
	verticies[1]=vertexTexNorm( bx, 0 , -bz,  0, -1, 0,  0, 1);
	verticies[2]=vertexTexNorm( bx, 0 ,  bz,  0, -1, 0,  0, 1);
	verticies[3]=vertexTexNorm(-bx, 0 ,  bz,  0, -1, 0,  0, 1);
	indexes[polyCount++] = 0; indexes[polyCount++] = 1; indexes[polyCount++] = 2;
	indexes[polyCount++] = 0; indexes[polyCount++] = 2; indexes[polyCount++] = 3;
	
	verticies[4]=vertexTexNorm(-bx, 0 , -bz,  0, 0, -1,  0, 1);
	verticies[5]=vertexTexNorm( bx, 0 , -bz,  0, 0, -1,  0, 1);
	verticies[6]=vertexTexNorm( bx, by, -bz,  0, 0, -1,  0, 1);
	verticies[7]=vertexTexNorm(-bx, by, -bz,  0, 0, -1,  0, 1);
	indexes[polyCount++] = 4; indexes[polyCount++] = 6; indexes[polyCount++] = 5;
	indexes[polyCount++] = 4; indexes[polyCount++] = 7; indexes[polyCount++] = 6;
	
	verticies[8]= vertexTexNorm(-bx, 0 , bz,  0, 0, 1,  0, 1);
	verticies[9]= vertexTexNorm( bx, 0 , bz,  0, 0, 1,  0, 1);
	verticies[10]=vertexTexNorm( bx, by, bz,  0, 0, 1,  0, 1);
	verticies[11]=vertexTexNorm(-bx, by, bz,  0, 0, 1,  0, 1);
	indexes[polyCount++] = 10; indexes[polyCount++] = 8; indexes[polyCount++] = 9;
	indexes[polyCount++] = 10; indexes[polyCount++] = 11; indexes[polyCount++] = 8;	
	
	verticies[12]=vertexTexNorm(bx, 0 , -bz,  1, 0, 0,  0, 1);
	verticies[13]=vertexTexNorm(bx, 0 ,  bz,  1, 0, 0,  0, 1);
	verticies[14]=vertexTexNorm(bx, by,  bz,  1, 0, 0,  0, 1);
	verticies[15]=vertexTexNorm(bx, by, -bz,  1, 0, 0,  0, 1);
	indexes[polyCount++] = 12; indexes[polyCount++] = 14; indexes[polyCount++] = 13;
	indexes[polyCount++] = 12; indexes[polyCount++] = 15; indexes[polyCount++] = 14;
	
	verticies[16]=vertexTexNorm(-bx, 0 , -bz,  -1, 0, 0,  0, 1);
	verticies[17]=vertexTexNorm(-bx, 0 ,  bz,  -1, 0, 0,  0, 1);
	verticies[18]=vertexTexNorm(-bx, by,  bz,  -1, 0, 0,  0, 1);
	verticies[19]=vertexTexNorm(-bx, by, -bz,  -1, 0, 0,  0, 1);
	indexes[polyCount++] = 18; indexes[polyCount++] = 19; indexes[polyCount++] = 17;
	indexes[polyCount++] = 19; indexes[polyCount++] = 16; indexes[polyCount++] = 17;

	
	verticies[20]=vertexTexNorm(-size.x, by, -size.z,  -1, 0, 0,  0, 1);
	verticies[21]=vertexTexNorm( size.x, by, -size.z,  -1, 0, 0,  0, 1);
	verticies[22]=vertexTexNorm( size.x, by,  size.z,  -1, 0, 0,  0, 1);
	verticies[23]=vertexTexNorm(-size.x, by,  size.z,  -1, 0, 0,  0, 1);
	verticies[24]=vertexTexNorm(0, size.y, 0,  -1, 0, 0,  0, 1);
	
	indexes[polyCount++] = 20; indexes[polyCount++] = 24; indexes[polyCount++] = 21;
	indexes[polyCount++] = 21; indexes[polyCount++] = 24; indexes[polyCount++] = 22;
	indexes[polyCount++] = 22; indexes[polyCount++] = 24; indexes[polyCount++] = 23;
	indexes[polyCount++] = 23; indexes[polyCount++] = 24; indexes[polyCount++] = 20;
	indexes[polyCount++] = 20; indexes[polyCount++] = 21; indexes[polyCount++] = 22;
	indexes[polyCount++] = 20; indexes[polyCount++] = 22; indexes[polyCount++] = 23;
	
	polyCount /= 3;
	vertCount = 25;
}