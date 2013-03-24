#ifndef ENGINE_SCENE_STUFF_H
#define ENGINE_SCENE_STUFF_H

#include "../../util/utils.h"

struct cScene;
struct cObject;
struct grRender3DObjectMesh;
struct grTexture;
struct grMaterial;
struct grSurfaceMaterial;
struct phRigidObject;
struct phStaticObject;

struct cSceneStuff
{
	enum cv { nVertexBufferSize = 4096, nIndexesBufferSize = 12288 };

	cScene* mScene;

	vertexTexNorm* mVertexBuffer;
	int*           mIndexesBuffer;

//functions
	cSceneStuff(cScene* scene);
	~cSceneStuff();

	
	cObject* createRigidWoodBox(const vec3& pos, const vec3& size, const mat3x3& orient = nullMatr() );
	cObject* createStaticWoodBox(const vec3& pos, const vec3& size, const mat3x3& orient = nullMatr() );

	cObject* createBoxMesh(vec3& pos, vec3& size, mat3x3& orient = nullMatr());

//render
	grRender3DObjectMesh* createMesh(unsigned int vertexCount, unsigned int polyCount);
	grRender3DObjectMesh* addBoxMesh(grRender3DObjectMesh* mesh, const vec3& size, grSurfaceMaterial* material, 
		                             const vec3& pos = vec3(0));
	grSurfaceMaterial* createSurfaceMaterial(grTexture* texture, grMaterial* material);
	grTexture* createTexture(const char* fileName);
	grMaterial* getMaterial(const char* materialName);

//physics
	phRigidObject* createPhysicsRigidBody(const vec3& pos, const mat3x3& orient, float mass, const mat3x3& inertia);
	phStaticObject* createPhysicsStaticBody(const vec3& pos, const mat3x3& orient);
	phRigidObject* addBoxCollisionGeometry(phRigidObject* rigidObject, const vec3& size, const vec3& offset = vec3(0),
		                                   const mat3x3& orient = nullMatr());
	phStaticObject* addBoxCollisionGeometry(phStaticObject* staticObject, const vec3& size, const vec3& offset = vec3(0),
		                                   const mat3x3& orient = nullMatr());
};

#endif //ENGINE_SCENE_STUFF_H