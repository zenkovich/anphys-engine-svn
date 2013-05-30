#include "vehicle.h"

#include <algorithm>

#include "vehicle_component.h"
#include "physics/objects/rigid_object.h"
#include "physics/scenes/physics_scene.h"
#include "physics/CD/collision_manager.h"
#include "physics/CD/collision_geometry/collision.h"
#include "physics/CD/collision_geometry/collision_point.h"
#include "util/debug/render_stuff.h"


phVehicle::phVehicle():phRigidObject()
{
}

phVehicle::~phVehicle()
{
	removeAllComponents();
}

void phVehicle::addComponent( phVehicleComponent* component )
{
	mComponents.push_back(component);
	component->mVehicle = this;
}

void phVehicle::removeComponent( phVehicleComponent* component )
{
	ComponentsList::iterator fnd = std::find(mComponents.begin(), mComponents.end(), component);
	if (fnd == mComponents.end())
		return;

	safe_release(component);
	mComponents.erase(fnd);
}

void phVehicle::removeAllComponents()
{
	for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		safe_release(*it);

	mComponents.clear();
}

void phVehicle::preSolve( float dt )
{
	phRigidObject::preSolve(dt);

	for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		(*it)->preSolve(dt);
}

void phVehicle::postSolve( float dt )
{	
	for (int i = 0; i < 1; i++)
	{
		for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it)->solve(dt);
	}

	phRigidObject::postSolve(dt);

	for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		(*it)->postSolve(dt);

	for (unsigned int i = 0; i < mPolygonsBufferCount; i++)
	{
		getRenderStuff().addRedArrow(mPolygonsBuffer[i]->pa->mPosition, mPolygonsBuffer[i]->pb->mPosition);
		getRenderStuff().addGreenArrow(mPolygonsBuffer[i]->pb->mPosition, mPolygonsBuffer[i]->pc->mPosition);
		getRenderStuff().addBlueArrow(mPolygonsBuffer[i]->pc->mPosition, mPolygonsBuffer[i]->pa->mPosition);

		getRenderStuff().addBlueArrow((mPolygonsBuffer[i]->pa->mPosition + mPolygonsBuffer[i]->pb->mPosition)*0.5f,
			                          (mPolygonsBuffer[i]->pa->mPosition + mPolygonsBuffer[i]->pb->mPosition)*0.5f + 
									  mPolygonsBuffer[i]->nab.normalize()*0.3f);

		getRenderStuff().addBlueArrow((mPolygonsBuffer[i]->pb->mPosition + mPolygonsBuffer[i]->pc->mPosition)*0.5f,
			                          (mPolygonsBuffer[i]->pb->mPosition + mPolygonsBuffer[i]->pc->mPosition)*0.5f + 
									  mPolygonsBuffer[i]->nbc.normalize()*0.3f);

		getRenderStuff().addBlueArrow((mPolygonsBuffer[i]->pc->mPosition + mPolygonsBuffer[i]->pa->mPosition)*0.5f,
			                          (mPolygonsBuffer[i]->pc->mPosition + mPolygonsBuffer[i]->pa->mPosition)*0.5f + 
									  mPolygonsBuffer[i]->nca.normalize()*0.3f);

		getRenderStuff().addBlueArrow((mPolygonsBuffer[i]->pa->mPosition + mPolygonsBuffer[i]->pb->mPosition + mPolygonsBuffer[i]->pc->mPosition)/3.0f,
			                          (mPolygonsBuffer[i]->pa->mPosition + mPolygonsBuffer[i]->pb->mPosition + mPolygonsBuffer[i]->pc->mPosition)/3.0f + 
									  mPolygonsBuffer[i]->norm);
	}
}

phVehicleComponent* phVehicle::getComponent( const std::string& id )
{
	for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		if ((*it)->mId == id)
			return *it;

	return NULL;
}
