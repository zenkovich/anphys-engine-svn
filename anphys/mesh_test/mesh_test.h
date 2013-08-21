#ifndef MESH_TEST_H
#define MESH_TEST_H

#include <vector>

#include "engine/engine_incl.h"

class cMeshTest
{
public:
	typedef std::vector<vec3> VecArr;
	typedef std::vector<poly3> PolyArr;
	
	VecArr  mMainMeshVericies;
	PolyArr mMainMeshPolygons;
	
	VecArr  mSecondaryMeshVericies;
	PolyArr mSecondaryMeshPolygons;

	void generateMainPlaneMesh(const vec3& size, const int segx, const int segy);
	
	void generateSecondaryPlaneMesh(const vec3& size, const int segx, const int segy);
	void generateSecondaryTorusMesh(const vec3& size, const int circleSegs, const int circles);
	void generateSecondarySphereMesh(const vec3& size, const int circleSegs, const int circles);
	void generateSecondaryCubeMesh(const vec3& size, const int segsx, const int segsy, const int segsz);
	
	void randomizeMainMesh(const vec3& range);
	void randomizeSecondaryMesh(const vec3& range);

	void processMeshMerge(const vec3& secMeshOffs, const mat3x3& secMeshOrient);

protected:
	void createPlaneMesh(VecArr& verticies, PolyArr& polygons, const vec3& size, const int segx, const int segy);
	void createTorusMesh(VecArr& verticies, PolyArr& polygons, const vec3& size, const int circleSegs, const int circles);
	void createSphereMesh(VecArr& verticies, PolyArr& polygons, const vec3& size, const int circleSegs, const int circles);
	void createCubeMesh(VecArr& verticies, PolyArr& polygons, const vec3& size, const int segsx, const int segsy, const int segsz);
	void randomizeMesh(VecArr& verticies, PolyArr& polygons, const vec3& range);
};

#endif //MESH_TEST_H