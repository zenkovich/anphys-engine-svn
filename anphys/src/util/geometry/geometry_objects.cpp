#include "geometry_objects.h"



void boxGeometry(vertexTexNorm* verticies, int &vertCount, int* indexes, int &polyCount, vec3 size)
{
	polyCount = 0;
	vec3 hs = size*0.5f;
	verticies[0] = vertexTexNorm(-hs.x, -hs.y, -hs.z, 0, 0, -1, 0, 0);
	verticies[1] = vertexTexNorm( hs.x, -hs.y, -hs.z, 0, 0, -1, 0, 1);
	verticies[2] = vertexTexNorm( hs.x,  hs.y, -hs.z, 0, 0, -1, 1, 1);
	verticies[3] = vertexTexNorm(-hs.x,  hs.y, -hs.z, 0, 0, -1, 1, 0);
	indexes[polyCount++] = 0; indexes[polyCount++] = 1; indexes[polyCount++] = 2;
	indexes[polyCount++] = 0; indexes[polyCount++] = 2; indexes[polyCount++] = 3;
	
	verticies[4] = vertexTexNorm(-hs.x, -hs.y, hs.z, 0, 0, 1, 0, 0);
	verticies[5] = vertexTexNorm( hs.x, -hs.y, hs.z, 0, 0, 1, 0, 1);
	verticies[6] = vertexTexNorm( hs.x,  hs.y, hs.z, 0, 0, 1, 1, 1);
	verticies[7] = vertexTexNorm(-hs.x,  hs.y, hs.z, 0, 0, 1, 1, 0);
	indexes[polyCount++] = 6; indexes[polyCount++] = 5; indexes[polyCount++] = 4;
	indexes[polyCount++] = 7; indexes[polyCount++] = 6; indexes[polyCount++] = 4;
	
	verticies[8] = vertexTexNorm(-hs.x, -hs.y, -hs.z, -1, 0, 0, 0, 0);
	verticies[9] = vertexTexNorm(-hs.x,  hs.y, -hs.z, -1, 0, 0, 0, 1);
	verticies[10] = vertexTexNorm(-hs.x,  hs.y,  hs.z, -1, 0, 0, 1, 1);
	verticies[11] = vertexTexNorm(-hs.x, -hs.y,  hs.z, -1, 0, 0, 1, 0);
	indexes[polyCount++] = 8; indexes[polyCount++] = 9; indexes[polyCount++] = 10;
	indexes[polyCount++] = 8; indexes[polyCount++] = 10; indexes[polyCount++] = 11;
	
	verticies[12] = vertexTexNorm(hs.x, -hs.y, -hs.z, 1, 0, 0, 0, 0);
	verticies[13] = vertexTexNorm(hs.x,  hs.y, -hs.z, 1, 0, 0, 0, 1);
	verticies[14] = vertexTexNorm(hs.x,  hs.y,  hs.z, 1, 0, 0, 1, 1);
	verticies[15] = vertexTexNorm(hs.x, -hs.y,  hs.z, 1, 0, 0, 1, 0);
	indexes[polyCount++] = 13; indexes[polyCount++] = 12; indexes[polyCount++] = 14;
	indexes[polyCount++] = 14; indexes[polyCount++] = 12; indexes[polyCount++] = 15;
	
	verticies[16] = vertexTexNorm(-hs.x, -hs.y, -hs.z, 0, -1, 0, 0, 0);
	verticies[17] = vertexTexNorm( hs.x, -hs.y, -hs.z, 0, -1, 0, 0, 1);
	verticies[18] = vertexTexNorm( hs.x, -hs.y,  hs.z, 0, -1, 0, 1, 1);
	verticies[19] = vertexTexNorm(-hs.x, -hs.y,  hs.z, 0, -1, 0, 1, 0);
	indexes[polyCount++] = 17; indexes[polyCount++] = 16; indexes[polyCount++] = 18;
	indexes[polyCount++] = 18; indexes[polyCount++] = 16; indexes[polyCount++] = 19;
	
	verticies[20] = vertexTexNorm(-hs.x, hs.y, -hs.z, 0, 1, 0, 0, 0);
	verticies[21] = vertexTexNorm( hs.x, hs.y, -hs.z, 0, 1, 0, 0, 1);
	verticies[22] = vertexTexNorm( hs.x, hs.y,  hs.z, 0, 1, 0, 1, 1);
	verticies[23] = vertexTexNorm(-hs.x, hs.y,  hs.z, 0, 1, 0, 1, 0);
	indexes[polyCount++] = 20; indexes[polyCount++] = 21; indexes[polyCount++] = 22;
	indexes[polyCount++] = 20; indexes[polyCount++] = 22; indexes[polyCount++] = 23;

	polyCount /= 3;
	vertCount = 24;
}

void arrowGeometry(vertexTexNorm* verticies, int &vertCount, int* indexes, int &polyCount, vec3 size)
{
	float x=size.x*0.2f;
	float y=size.y*0.3f;
	float z=size.z*0.2f;
	
	float y2=size.y*0.5f;

	verticies[0]=vertexTexNorm(-x,-y2+y2,z,0,0,1,0,1);
	verticies[1]=vertexTexNorm(-x,y+y2,z,0,0,1,1,1);
	verticies[2]=vertexTexNorm(x,y+y2,z,0,0,1,1,0);
	verticies[3]=vertexTexNorm(x,-y2+y2,z,0,0,1,0,0);
	indexes[polyCount++] = 1; indexes[polyCount++] = 2; indexes[polyCount++] = 0;
	indexes[polyCount++] = 0; indexes[polyCount++] = 2; indexes[polyCount++] = 3;
	
	verticies[4]=vertexTexNorm(-x,-y2+y2,-z,0,0,-1,0,1);
	verticies[5]=vertexTexNorm(-x,y+y2,-z,0,0,-1,1,1);
	verticies[6]=vertexTexNorm(x,y+y2,-z,0,0,-1,1,0);
	verticies[7]=vertexTexNorm(x,-y2+y2,-z,0,0,-1,0,0);
	indexes[polyCount++] = 6; indexes[polyCount++] = 5; indexes[polyCount++] = 4;
	indexes[polyCount++] = 7; indexes[polyCount++] = 6; indexes[polyCount++] = 4;

	verticies[8]= vertexTexNorm(x,-y2+y2,z,1,0,0,0,1);
	verticies[9]= vertexTexNorm(x,y+y2,z,1,0,0,1,1);
	verticies[10]=vertexTexNorm(x,y+y2,-z,1,0,0,1,0);
	verticies[11]=vertexTexNorm(x,-y2+y2,-z,1,0,0,0,0);
	indexes[polyCount++] = 8; indexes[polyCount++] = 9; indexes[polyCount++] = 10;
	indexes[polyCount++] = 9; indexes[polyCount++] = 10; indexes[polyCount++] = 11;
	
	verticies[12]=vertexTexNorm(-x,-y2+y2,z,-1,0,0,0,1);
	verticies[13]=vertexTexNorm(-x,y+y2,z,-1,0,0,1,1);
	verticies[14]=vertexTexNorm(-x,y+y2,-z,-1,0,0,1,0);
	verticies[15]=vertexTexNorm(-x,-y2+y2,-z,-1,0,0,0,0);
	indexes[polyCount++] = 14; indexes[polyCount++] = 13; indexes[polyCount++] = 12;
	indexes[polyCount++] = 15; indexes[polyCount++] = 14; indexes[polyCount++] = 12;
	
	verticies[16]=vertexTexNorm(-x,y+y2,z,0,1,0,0,1);
	verticies[17]=vertexTexNorm(-x,y+y2,-z,0,1,0,1,1);
	verticies[18]=vertexTexNorm(x,y+y2,-z,0,1,0,1,0);
	verticies[19]=vertexTexNorm(x,y+y2,z,0,1,0,0,0);
	indexes[polyCount++] = 16; indexes[polyCount++] = 17; indexes[polyCount++] = 18;
	indexes[polyCount++] = 16; indexes[polyCount++] = 18; indexes[polyCount++] = 19;
	
	verticies[20]=vertexTexNorm(-x,-y2+y2,z,0,-1,0,0,1);
	verticies[21]=vertexTexNorm(-x,-y2+y2,-z,0,-1,0,1,1);
	verticies[22]=vertexTexNorm(x,-y2+y2,-z,0,-1,0,1,0);
	verticies[23]=vertexTexNorm(x,-y2+y2,z,0,-1,0,0,0);
	indexes[polyCount++] = 22; indexes[polyCount++] = 21; indexes[polyCount++] = 20;
	indexes[polyCount++] = 23; indexes[polyCount++] = 22; indexes[polyCount++] = 20;

//head	
	
	float hx=size.x*0.5f;
	float hy=size.y*0.5f;
	float hz=size.z*0.5f;

	verticies[24]=vertexTexNorm(-hx,y+y2,hz,0,0,1,0,1);
	verticies[25]=vertexTexNorm(0,hy+y2,0,0,0,1,1,1);
	verticies[26]=vertexTexNorm(0,hy+y2,0,0,0,1,1,0);
	verticies[27]=vertexTexNorm(hx,y+y2,hz,0,0,1,0,0);
	indexes[polyCount++] = 25; indexes[polyCount++] = 26; indexes[polyCount++] = 24;
	indexes[polyCount++] = 24; indexes[polyCount++] = 26; indexes[polyCount++] = 27;
	
	verticies[28]=vertexTexNorm(-hx,y+y2,-hz,0,0,-1,0,1);
	verticies[29]=vertexTexNorm(0,hy+y2,0,0,0,-1,1,1);
	verticies[30]=vertexTexNorm(0,hy+y2,0,0,0,-1,1,0);
	verticies[31]=vertexTexNorm(hx,y+y2,-hz,0,0,-1,0,0);
	indexes[polyCount++] = 30; indexes[polyCount++] = 29; indexes[polyCount++] = 28;
	indexes[polyCount++] = 31; indexes[polyCount++] = 30; indexes[polyCount++] = 28;
	
	verticies[32]= vertexTexNorm(hx,y+y2,hz,1,0,0,0,1);
	verticies[33]= vertexTexNorm(0,hy+y2,0,1,0,0,1,1);
	verticies[34]=vertexTexNorm(0,hy+y2,0,1,0,0,1,0);
	verticies[35]=vertexTexNorm(hx,y+y2,-hz,1,0,0,0,0);
	indexes[polyCount++] = 32; indexes[polyCount++] = 33; indexes[polyCount++] = 34;
	indexes[polyCount++] = 32; indexes[polyCount++] = 34; indexes[polyCount++] = 35;
	
	verticies[36]=vertexTexNorm(-hx,y+y2,hz,-1,0,0,0,1);
	verticies[37]=vertexTexNorm(0,hy+y2,0,-1,0,0,1,1);
	verticies[38]=vertexTexNorm(0,hy+y2,0,-1,0,0,1,0);
	verticies[39]=vertexTexNorm(-hx,y+y2,-hz,-1,0,0,0,0);
	indexes[polyCount++] = 38; indexes[polyCount++] = 37; indexes[polyCount++] = 36;
	indexes[polyCount++] = 39; indexes[polyCount++] = 38; indexes[polyCount++] = 36;
	
	verticies[40]=vertexTexNorm(0,hy+y2,0,0,1,0,0,1);
	verticies[41]=vertexTexNorm(0,hy+y2,0,0,1,0,1,1);
	verticies[42]=vertexTexNorm(0,hy+y2,0,0,1,0,1,0);
	verticies[43]=vertexTexNorm(0,hy+y2,0,0,1,0,0,0);
	indexes[polyCount++] = 40; indexes[polyCount++] = 41; indexes[polyCount++] = 42;
	indexes[polyCount++] = 40; indexes[polyCount++] = 42; indexes[polyCount++] = 43;
	
	verticies[44]=vertexTexNorm(-hx,y+y2,hz,0,-1,0,0,1);
	verticies[45]=vertexTexNorm(-hx,y+y2,-hz,0,-1,0,1,1);
	verticies[46]=vertexTexNorm(hx,y+y2,-hz,0,-1,0,1,0);
	verticies[47]=vertexTexNorm(hx,y+y2,hz,0,-1,0,0,0);
	indexes[polyCount++] = 46; indexes[polyCount++] = 45; indexes[polyCount++] = 44;
	indexes[polyCount++] = 47; indexes[polyCount++] = 46; indexes[polyCount++] = 44;
	
	polyCount /= 3;
	vertCount = 48;
}