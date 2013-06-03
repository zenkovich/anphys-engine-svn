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
	int maxCollisionPoints = 50;

	for (int i = 0; i < maxCollisionPoints; i++)
		mFreeCollisionPoints.push_back(new phCollisionPoint());
}

phVehicle::~phVehicle()
{
	removeAllComponents();

	for (CollisionPointsList::iterator it = mFreeCollisionPoints.begin(); it != mFreeCollisionPoints.end(); it++)
	{
		safe_release(*it);
	}
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

	solveCollisions(dt);
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

	updateCollisionGeometry();
	checkCollisions();

	for (unsigned int i = 0; i < mPolygonsBufferCount; i++)
	{
		getRenderStuff().addRedArrow(mPolygonsBuffer[i]->pa->mPosition, mPolygonsBuffer[i]->pb->mPosition);
		getRenderStuff().addGreenArrow(mPolygonsBuffer[i]->pb->mPosition, mPolygonsBuffer[i]->pc->mPosition);
		getRenderStuff().addBlueArrow(mPolygonsBuffer[i]->pc->mPosition, mPolygonsBuffer[i]->pa->mPosition);

		/*getRenderStuff().addBlueArrow((mPolygonsBuffer[i]->pa->mPosition + mPolygonsBuffer[i]->pb->mPosition)*0.5f,
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
									  mPolygonsBuffer[i]->norm);*/
	}
}

phVehicleComponent* phVehicle::getComponent( const std::string& id )
{
	for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		if ((*it)->mId == id)
			return *it;

	return NULL;
}

void phVehicle::checkCollisions()
{
	mActiveCollisionPoints.clear();

	for (PointsList::iterator it = mCollisionGeometryPoints.begin(); it != mCollisionGeometryPoints.end(); ++it)
	{
		phCollisionGeometryVertex* vertex = &(*it);

		phCollisionPoint* cpoint = vertex->mCollisionPoint;
		bool fromFreeList = false;

		if (!cpoint)
		{
			cpoint = mFreeCollisionPoints[0];
			cpoint->reset();
			fromFreeList = true;
		}

		bool isColliding = false;
		float minDepth = 999.0f;

		for (int i = 0; i < mPolygonsBufferCount; i++)
		{
			lPolygon* polygon = mPolygonsBuffer[i];

			if (polygon->isIntersect(vertex->mGlobalPos, &cpoint->mPoint, &cpoint->mNormal, &cpoint->mDepth))
			{
				if (minDepth > cpoint->mDepth)
				{
					minDepth = cpoint->mDepth;
					isColliding = true;
				}
			}
		}

		if (isColliding)
		{
			mActiveCollisionPoints.push_back(cpoint);
			vertex->mCollisionPoint = cpoint;
		}
		else
		{
			vertex->mCollisionPoint = NULL;
		}

		if (fromFreeList && isColliding)
		{
			mFreeCollisionPoints.erase(mFreeCollisionPoints.begin());
		}
		if (!fromFreeList && !isColliding)
		{
			mFreeCollisionPoints.push_back(cpoint);
		}
	}
}

void phVehicle::solveCollisions( float dt )
{
	float E = 0.3f, Mu = 0.5f, biasERP = 0.8f;
	int iterations = 10;
	float invDt = 1.0f/dt;

	for (CollisionPointsList::iterator jt = mActiveCollisionPoints.begin(); jt != mActiveCollisionPoints.end(); ++jt)
	{
		phCollisionPoint* collisionPoint = *jt;

		float contactDepth = collisionPoint->mDepth - 0.05f;
		if (contactDepth > 0.0f)
		{
			collisionPoint->mBiasImpulse = contactDepth*invDt*biasERP;
		}
		else collisionPoint->mBiasImpulse = 0.0f;

		collisionPoint->t1 = collisionPoint->mNormal^vec3(0, 0, 1);
		if (collisionPoint->t1*collisionPoint->t1 < 0.0001f)
			collisionPoint->t1 = collisionPoint->mNormal^vec3(0, 1, 0);

		collisionPoint->t2 = collisionPoint->mNormal^collisionPoint->t1;
			
		vec3 imp = collisionPoint->mNormal*collisionPoint->J + collisionPoint->t1*collisionPoint->Jf1 + 
				                                                collisionPoint->t2*collisionPoint->Jf2;
			
		applyImpulse(collisionPoint->mPoint, imp);

		getRenderStuff().addBlueArrow(collisionPoint->mPoint, collisionPoint->mPoint + collisionPoint->mNormal);

		vec3 ra = collisionPoint->mPoint - mPosition;
			
		vec3 n1 = collisionPoint->mNormal;            collisionPoint->n1 = n1;
		vec3 w1 = collisionPoint->mNormal^ra;         collisionPoint->w1 = w1;

		float b = n1*n1*mInvMass + w1*(w1*mInvWorldInertia);
		collisionPoint->B = 1.0f/b;

				
		vec3 f1n1 = collisionPoint->t1;            collisionPoint->f1n1 = f1n1;
		vec3 f1w1 = collisionPoint->t1^ra;         collisionPoint->f1w1 = f1w1;
				
		float f1b = f1n1*f1n1*mInvMass + f1w1*(f1w1*mInvWorldInertia);
		collisionPoint->Bf1 = 1.0f/f1b;
				
		vec3 f2n1 = collisionPoint->t2;            collisionPoint->f2n1 = f2n1;
		vec3 f2w1 = collisionPoint->t2^ra;         collisionPoint->f2w1 = f2w1;

		float f2b = f2n1*f2n1*mInvMass + f2w1*(f2w1*mInvWorldInertia);
		collisionPoint->Bf2 = 1.0f/f2b;
	}

	for (int i = 0; i < iterations; i++)
	{
		for (CollisionPointsList::iterator jt = mActiveCollisionPoints.begin(); jt != mActiveCollisionPoints.end(); ++jt)
		{
			phCollisionPoint* collisionPoint = *jt;
				
			vec3 ra = collisionPoint->mPoint - mPosition;
			
			vec3 n1 = collisionPoint->n1;
			vec3 w1 = collisionPoint->w1;

			float a = n1*mVelocity + w1*mAngularVelocity;
			float b =collisionPoint->B;

			float lambda = -a*(E + 1.0f)*b;

			collisionPoint->J += lambda;
			if(collisionPoint->J < 0.0f)
			{
				lambda += (0.0f - collisionPoint->J);
				collisionPoint->J = 0.0f;
			}

			applyImpulse(collisionPoint->mPoint, collisionPoint->mNormal*lambda);
				
			vec3 f1n1 = collisionPoint->f1n1;
			vec3 f1w1 = collisionPoint->f1w1;
				
			float f1a = f1n1*mVelocity + f1w1*mAngularVelocity;
			float f1b = collisionPoint->Bf1;

			float f1lambda = -f1a*f1b;
				
			vec3 f2n1 = collisionPoint->f2n1;
			vec3 f2w1 = collisionPoint->f2w1;
				
			float f2a = f2n1*mVelocity + f2w1*mAngularVelocity;
			float f2b = collisionPoint->Bf2;

			float f2lambda = -f2a*f2b;
				
			float f1accumulated = f1lambda + collisionPoint->Jf1;
			float f2accumulated = f2lambda + collisionPoint->Jf2;
			float maxfriction = collisionPoint->J*Mu;

			if (f1accumulated*f1accumulated + f2accumulated*f2accumulated > maxfriction*maxfriction)
			{
				float invLen = 1.0f/sqrtf(f1lambda*f1lambda + f2lambda*f2lambda);
				f1lambda = f1lambda*invLen*maxfriction - collisionPoint->Jf1;
				f2lambda = f2lambda*invLen*maxfriction - collisionPoint->Jf2;
			}
				
			collisionPoint->Jf1 += f1lambda;
			collisionPoint->Jf2 += f2lambda;

			vec3 Jf = collisionPoint->t1*f1lambda + collisionPoint->t2*f2lambda;

			applyImpulse(collisionPoint->mPoint, Jf);

			
			float biasA = n1*mBiasVelocity + w1*mBiasAngularVelocity;

			float biasLambda = -(biasA - collisionPoint->mBiasImpulse)*b;

			if (biasLambda > 0.0f)					
				applyBiasImpulse(collisionPoint->mPoint, collisionPoint->mNormal*biasLambda);
		}
	}
}

void phVehicle::updateCollisionGeometry()
{
	for (PointsList::iterator it = mCollisionGeometryPoints.begin(); it != mCollisionGeometryPoints.end(); ++it)
	{
		phCollisionGeometryVertex* vertex = &(*it);
		vertex->mGlobalPos = vertex->mLocalPos*mOrient + mPosition;
		getRenderStuff().addGreenCube(vertex->mGlobalPos);
	}
}
