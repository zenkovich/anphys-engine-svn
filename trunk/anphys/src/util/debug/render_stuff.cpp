#include "render_stuff.h"

template<> cRenderStuff* cSingleton<cRenderStuff>::mInstance = NULL;

void cRenderStuff::initialize( grRenderSceneBaseInterface* scene )
{
	grRender* render = scene->mSceneManager->mRender;

	vertexTexNorm* verticies = new vertexTexNorm[120];
	int* indexes = new int[360];
	int vertCount = 0;
	int polyCount = 0;

	arrowGeometry(verticies, vertCount, indexes, polyCount, vec3(1));

	mRedArrows = static_cast<grRenderMulti3DObject*>(scene->mObjects->createObject(new grRenderMulti3DObject()));
	mRedArrows->setRenderObject(new grRender3DObjectMesh(scene->mObjects, vertCount, polyCount));
	mRedArrows->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		render->mSurfaceMaterials->getSurfaceMaterial("redMaterial"));
	

	mGreenArrows = static_cast<grRenderMulti3DObject*>(scene->mObjects->createObject(new grRenderMulti3DObject()));
	mGreenArrows->setRenderObject(new grRender3DObjectMesh(scene->mObjects, vertCount, polyCount));
	mGreenArrows->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		render->mSurfaceMaterials->getSurfaceMaterial("greenMaterial"));

	mBlueArrows = static_cast<grRenderMulti3DObject*>(scene->mObjects->createObject(new grRenderMulti3DObject()));
	mBlueArrows->setRenderObject(new grRender3DObjectMesh(scene->mObjects, vertCount, polyCount));
	mBlueArrows->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		render->mSurfaceMaterials->getSurfaceMaterial("blueMaterial"));
	
	boxGeometry(verticies, vertCount, indexes, polyCount, vec3(0.1f));	

	mRedCubes = static_cast<grRenderMulti3DObject*>(scene->mObjects->createObject(new grRenderMulti3DObject()));
	mRedCubes->setRenderObject(new grRender3DObjectMesh(scene->mObjects, vertCount, polyCount));
	mRedCubes->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		render->mSurfaceMaterials->getSurfaceMaterial("redMaterial"));

	mGreenCubes = static_cast<grRenderMulti3DObject*>(scene->mObjects->createObject(new grRenderMulti3DObject()));
	mGreenCubes->setRenderObject(new grRender3DObjectMesh(scene->mObjects, vertCount, polyCount));
	mGreenCubes->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		render->mSurfaceMaterials->getSurfaceMaterial("greenMaterial"));

	mBlueCubes = static_cast<grRenderMulti3DObject*>(scene->mObjects->createObject(new grRenderMulti3DObject()));
	mBlueCubes->setRenderObject(new grRender3DObjectMesh(scene->mObjects, vertCount, polyCount));
	mBlueCubes->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		render->mSurfaceMaterials->getSurfaceMaterial("blueMaterial"));
	
	safe_release_arr(verticies);
	safe_release_arr(indexes);
}

gr3DObjectConditionContainer& cRenderStuff::addRedArrow( const vec3& start, const vec3& end )
{
	gr3DObjectConditionContainer* condition = grMultiVector(start, end);
	mRedArrows->pushObject(condition);	
	return *condition;
}

gr3DObjectConditionContainer& cRenderStuff::addGreenArrow( const vec3& start, const vec3& end )
{	
	gr3DObjectConditionContainer* condition = grMultiVector(start, end);
	mGreenArrows->pushObject(condition);
	return *condition;
}

gr3DObjectConditionContainer& cRenderStuff::addBlueArrow( const vec3& start, const vec3& end )
{
	gr3DObjectConditionContainer* condition = grMultiVector(start, end);
	mBlueArrows->pushObject(condition);
	return *condition;
}

gr3DObjectConditionContainer& cRenderStuff::addRedCube( const vec3& pos, const vec3& size /*= vec3(0.1f)*/ )
{
	gr3DObjectConditionContainer* condition = new gr3DObjectConditionContainer(nullMatr(), pos);
	mRedCubes->pushObject(condition);
	return *condition;
}

gr3DObjectConditionContainer& cRenderStuff::addGreenCube( const vec3& pos, const vec3& size /*= vec3(0.1f)*/ )
{
	gr3DObjectConditionContainer* condition = new gr3DObjectConditionContainer(nullMatr(), pos);
	mGreenCubes->pushObject(condition);
	return *condition;
}

gr3DObjectConditionContainer& cRenderStuff::addBlueCube( const vec3& pos, const vec3& size /*= vec3(0.1f)*/ )
{
	gr3DObjectConditionContainer* condition = new gr3DObjectConditionContainer(nullMatr(), pos);
	mBlueCubes->pushObject(condition);
	return *condition;
}
