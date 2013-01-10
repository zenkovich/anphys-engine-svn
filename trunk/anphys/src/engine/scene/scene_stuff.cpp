#include "scene_stuff.h"

//engine
#include "scene.h"
#include "object.h"

//components
#include "render3d_object_component.h"
#include "physics_rigid_body_object_component.h"
#include "physics_static_body_object_component.h"

//render
#include "../../render/render.h"
#include "../../render/render_3d_object_mesh.h"
#include "../../render/render_scene_base_interface.h"
#include "../../render/scene_manager.h"
#include "../../render/object_manager.h"
#include "../../render/texture.h"
#include "../../render/texture_manager.h"
#include "../../render/material.h"
#include "../../render/material_manager.h"
#include "../../render/surface_material.h"
#include "../../render/surface_material_manager.h"

//physics
#include "../../physics/rigid_object.h"
#include "../../physics/static_object.h"
#include "../../physics/physics_engine.h"
#include "../../physics/physics_scene.h"
#include "../../physics/collision_geometry.h"
#include "../../physics/collision_geometry_part.h"
#include "../../physics/box_collision_geometry.h"


cSceneStuff::cSceneStuff( cScene* scene ):mScene(scene)
{
	mVertexBuffer = new vertexTexNorm[nVertexBufferSize];
	mIndexesBuffer = new int[nIndexesBufferSize];
}

cSceneStuff::~cSceneStuff()
{
	safe_release_arr(mVertexBuffer);
	safe_release_arr(mIndexesBuffer);
}

cObject* cSceneStuff::createRigidWoodBox( const vec3& pos, const vec3& size, const mat3x3& orient /*= nullMatr() */ )
{
	cObject* res = new cObject;

	grRender3DObjectMesh* boxMesh = createMesh(128, 128);
	addBoxMesh(boxMesh, size, 
		createSurfaceMaterial(createTexture("../data/textures/wood.jpg"), getMaterial("whiteMaterial")));

	cRender3DObjectComponent* boxMeshComponent = new cRender3DObjectComponent(boxMesh);
	res->addComponent(boxMeshComponent);

	phRigidObject* rigidBoxObject = createPhysicsRigidBody(pos, orient, 200.0f, ScaledMatrix(450.0f, 450.0f, 450.0f));
	addBoxCollisionGeometry(rigidBoxObject, size);

	cPhysicsRigidBodyObjectComponent* rigidBoxObjectComponent = new cPhysicsRigidBodyObjectComponent(rigidBoxObject);

	res->addComponent(rigidBoxObjectComponent);

	return res;
}

cObject* cSceneStuff::createStaticWoodBox( const vec3& pos, const vec3& size, const mat3x3& orient /*= nullMatr() */ )
{
	cObject* res = new cObject;

	grRender3DObjectMesh* boxMesh = createMesh(128, 128);
	addBoxMesh(boxMesh, size, 
		createSurfaceMaterial(createTexture("../data/textures/wood.jpg"), getMaterial("whiteMaterial")));

	cRender3DObjectComponent* boxMeshComponent = new cRender3DObjectComponent(boxMesh);
	res->addComponent(boxMeshComponent);

	phStaticObject* staticBoxObject = createPhysicsStaticBody(pos, orient);
	addBoxCollisionGeometry(staticBoxObject, size);

	cPhysicsStaticBodyObjectComponent* staticBoxObjectComponent = new cPhysicsStaticBodyObjectComponent(staticBoxObject);

	res->addComponent(staticBoxObjectComponent);

	return res;
}

grRender3DObjectMesh* cSceneStuff::createMesh( unsigned int vertexCount, unsigned int polyCount )
{
	return static_cast<grRender3DObjectMesh*>(mScene->mRenderScene->mObjects->createObject(
		new grRender3DObjectMesh(NULL, vertexCount, polyCount)));
}

grRender3DObjectMesh* cSceneStuff::addBoxMesh( grRender3DObjectMesh* mesh, const vec3& size, grSurfaceMaterial* material, 
	                                           const vec3& pos /*= vec3(0)*/ )
{
	int vertCount = 0, polyCount = 0;

	boxGeometry(mVertexBuffer, vertCount, mIndexesBuffer, polyCount, size);
	for (unsigned int i = 0; i < (unsigned int)vertCount; i++)
	{
		mVertexBuffer[i].x += pos.x;
		mVertexBuffer[i].y += pos.y;
		mVertexBuffer[i].z += pos.z;
	}

	mesh->addPart(mVertexBuffer, vertCount, mIndexesBuffer, polyCount, material);
	return mesh;
}

grSurfaceMaterial* cSceneStuff::createSurfaceMaterial( grTexture* texture, grMaterial* material )
{
	grSurfaceMaterial* res = mScene->mRenderScene->mSceneManager->mRender->mSurfaceMaterials->addSurfaceMaterial(
		new grSurfaceMaterial());

	res->setMaterial(material);
	res->pushTexture(texture);
	res->setShadeModel(NULL);

	return res;
}

grTexture* cSceneStuff::createTexture( const char* fileName )
{
	return mScene->mRenderScene->mSceneManager->mRender->mTextures->createTexture(fileName);
}

grMaterial* cSceneStuff::getMaterial( const char* materialName )
{
	return mScene->mRenderScene->mSceneManager->mRender->mMaterials->getMaterial(materialName);
}

phRigidObject* cSceneStuff::createPhysicsRigidBody( const vec3& pos, const mat3x3& orient, float mass, const mat3x3& inertia )
{
	phCollisionGeometry* collisionGeometry = new phCollisionGeometry;
	phRigidObject* res = new phRigidObject(pos, orient, mass, inertia, collisionGeometry);
	mScene->mPhysicsScene->addObject(res);
	return res;
}

phRigidObject* cSceneStuff::addBoxCollisionGeometry( phRigidObject* rigidObject, const vec3& size, const vec3& offset /*= vec3(0)*/, const mat3x3& orient /*= nullMatr()*/ )
{
	phBoxCollisionGeometry* boxCollisionGeom = new phBoxCollisionGeometry(orient, offset, size);
	rigidObject->mCollisionGeometry->addPart(boxCollisionGeom);
	return rigidObject;
}

phStaticObject* cSceneStuff::addBoxCollisionGeometry( phStaticObject* staticObject, const vec3& size, const vec3& offset /*= vec3(0)*/, const mat3x3& orient /*= nullMatr()*/ )
{
	phBoxCollisionGeometry* boxCollisionGeom = new phBoxCollisionGeometry(orient, offset, size);
	staticObject->mCollisionGeometry->addPart(boxCollisionGeom);
	return staticObject;
}

cObject* cSceneStuff::createBoxMesh( vec3& pos, vec3& size, mat3x3& orient /*= nullMatr()*/ )
{	
	cObject* res = new cObject;

	grRender3DObjectMesh* boxMesh = createMesh(128, 128);
	addBoxMesh(boxMesh, size, 
		createSurfaceMaterial(createTexture("../data/textures/transparent.tga"), getMaterial("whiteMaterial")));

	cRender3DObjectComponent* boxMeshComponent = new cRender3DObjectComponent(boxMesh);
	boxMeshComponent->mRender3DObject->mPosition = pos;
	boxMeshComponent->mRender3DObject->mOrient = orient;
	res->addComponent(boxMeshComponent);

	return res;
}

phStaticObject* cSceneStuff::createPhysicsStaticBody( const vec3& pos, const mat3x3& orient )
{	
	phCollisionGeometry* collisionGeometry = new phCollisionGeometry;
	phStaticObject* res = new phStaticObject(pos, orient, collisionGeometry);
	mScene->mPhysicsScene->addObject(res);
	return res;
}
