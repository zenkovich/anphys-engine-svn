#ifndef MESH_TEST_H
#define MESH_TEST_H

#include <vector>

#include "engine/engine_incl.h"

struct grRender3DObjectMesh;

class cMeshTest
{
public:
	typedef std::vector<vertexTexNorm> VecArr;
	typedef std::vector<poly3> PolyArr;
	
	VecArr  mMainMeshVericies;
	PolyArr mMainMeshPolygons;
	
	VecArr  mSecondaryMeshVericies;
	PolyArr mSecondaryMeshPolygons;

	void generateMainPlaneMesh(const vec3& size, const int segx, const int segy);
	
	void generateSecondaryPlaneMesh(const vec3& size, const int segx, const int segy);
	void generateSecondaryTorusMesh(const vec3& size, const int circleSegs, const int circles);
	void generateSecondarySphereMesh(const vec3& size, const int circleSegs, const int circles);
	
	void randomizeMainMesh(const vec3& range);
	void randomizeSecondaryMesh(const vec3& range);

	void processMeshMerge(const vec3& secMeshOffs, const mat3x3& secMeshOrient);
	
	void fillMainMeshData(std::vector<grRender3DObjectMesh*>& renderMeshes, const char* materialName);
	void fillSecondaryMeshData(grRender3DObjectMesh* renderMesh, const char* materialName);

protected:
	void createPlaneMesh(VecArr& verticies, PolyArr& polygons, const vec3& size, const int segx, const int segy);
	void createTorusMesh(VecArr& verticies, PolyArr& polygons, const vec3& size, const int circleSegs, const int circles);
	void createSphereMesh(VecArr& verticies, PolyArr& polygons, const vec3& size, const int circleSegs, const int circles);
	void randomizeMesh(VecArr& verticies, PolyArr& polygons, const vec3& range);
	void fillRenderMesh(VecArr& verticies, PolyArr& polygons, grRender3DObjectMesh* renderMesh, const char* materialName);
	int fillMainMeshPart(int polyStartIdx, grRender3DObjectMesh* mesh, const char* materialName);
};

#endif //MESH_TEST_H