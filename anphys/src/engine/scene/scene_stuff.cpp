#include "scene_stuff.h"

//engine
#include "scene.h"

//components
#include "render3d_object_component.h"
#include "physics_rigid_body_object_component.h"

//render
#include "../../render/render_3d_object_mesh.h"
#include "../../render/render_scene_base_interface.h"
#include "../../render/object_manager.h"
#include "../../render/texture.h"
#include "../../render/texture_manager.h"
#include "../../render/surface_material.h"
#include "../../render/surface_material_manager.h"


cSceneStuff::cSceneStuff( cScene* scene ):mScene(scene)
{

}

cObject* cSceneStuff::createRigidWoodBox( const vec3& pos, const vec3& size, const mat3x3& orient /*= nullMatr() */ )
{
	grTexture* tex = mScene->mRenderScene->mSceneManager->mRender->mTextures->createTexture("../data/textures/wood.jpg");
	grSurfaceMaterial* smat = mScene->mRenderScene->mSceneManager->mRender->mSurfaceMaterials->addSurfaceMaterial(
		new grSurfaceMaterial());
	smat->setMaterial(mRender->mMaterials->getMaterial("whiteMaterial"));
	smat->pushTexture(tex);
	smat->setShadeModel(NULL);

	vertexTexNorm* verticies = new vertexTexNorm[120];
	int* indexes = new int[360];
	int vertCount = 0;
	int polyCount = 0;

	boxGeometry(verticies, vertCount, indexes, polyCount, vec3(1));

	grRender3DObjectMesh* boxMesh = static_cast<grRender3DObjectMesh*>(
		mScene->mRenderScene->mObjects->createObject(new grRender3DObjectMesh(NULL, vertCount, polyCount)));

	boxMesh->addPart(verticies, vertCount, indexes, polyCount, smat);
	boxMesh->mPosition = vec3(0,0,0);

	cObject* object = new cObject;
	object->addComponent(new cRender3DObjectComponent(boxMesh));

	phRigidObject* physObject = mScene->mPhysicsScene->addObject(new phRigidObject(pos, orient, mass, inertia, collisionGeom));

	object->addComponent(new cPhysicsRigidBodyObjectComponent(physObject));
	return mScene->addObject(object);
}
