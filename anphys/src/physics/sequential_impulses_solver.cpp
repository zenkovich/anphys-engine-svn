#include "sequential_impulses_solver.h"

#include "../util/debug/render_stuff.h"

#include "physics_scene.h"
#include "physics_object.h"
#include "collision_manager.h"
#include "collision.h"
#include "collision_point.h"
#include "collision_geometry.h"
#include "collision_geometry_part.h"


void phSequentialImpulsesSolver::solveConstraints( float performance, float dt )
{
	phCollisionManager* collisionManager = mScene->mCollisionManager;

	float invDt = 1.0f/dt;

	/*for (phCollisionManager::CollisionsList::ValuesList::iterator it = 
		 collisionManager->mCollisions->mValues.begin(); it != collisionManager->mCollisions->mValues.end(); ++it)
	{
		phCollision* collision = *it;
		
		phObject* objectA = collision->mObjectA;
		phObject* objectB = collision->mObjectB;

		for (phCollision::CollisionPointsList::ValuesList::iterator jt = collision->mPoints->mValues.begin();
			 jt != collision->mPoints->mValues.end(); ++jt)
		{
			phCollisionPoint* collisionPoint = *jt;

			collisionPoint->mBiasImpulse = collisionPoint->mNormal*
				(collisionPoint->mDepth*10.5f)*
				 (-objectA->getMass() - objectB->getMass());

			collisionPoint->mImpulse = collisionPoint->mNormal*
				(collisionPoint->mNormal*
				 (objectA->getVelocity(collisionPoint->mPoint) - objectB->getVelocity(collisionPoint->mPoint)) )*
				 (-objectA->getMass() - objectB->getMass())*-0.1f;

			collisionPoint->mImpulse = collisionPoint->mBiasImpulse*0.4f;

			getRenderStuff().addBlueArrow(collisionPoint->mPoint, collisionPoint->mPoint + collisionPoint->mImpulse);

			collisionPoint->mPartObjectA->mCollisionGeometry->mObject->applyImpulse(collisionPoint);

			collisionPoint->mPartObjectB->mCollisionGeometry->mObject->applyImpulse(collisionPoint);
		}
	}*/

	for (phCollisionManager::CollisionsList::ValuesList::iterator it = 
		 collisionManager->mCollisions->mValues.begin(); it != collisionManager->mCollisions->mValues.end(); ++it)
	{
		phCollision* collision = *it;
		
		phObject* objectA = collision->mObjectA;
		phObject* objectB = collision->mObjectB;

		for (phCollision::CollisionPointsList::ValuesList::iterator jt = collision->mPoints->mValues.begin();
			 jt != collision->mPoints->mValues.end(); ++jt)
		{
			phCollisionPoint* collisionPoint = *jt;

			vec3 ra = collisionPoint->mPoint - objectA->getPos();
			vec3 rb = collisionPoint->mPoint - objectB->getPos();

			vec3 Vab(0), sm(0);
			float massSum=0;
			
			Vab -= objectA->getVelocity(collisionPoint->mPoint);
			sm += ( (ra^collisionPoint->mNormal)*objectA->retInvertedInertia() )^ra;
			massSum += objectA->getInvertedMass();

			Vab += objectB->getVelocity(collisionPoint->mPoint);
			sm += ( (rb^collisionPoint->mNormal)*objectB->retInvertedInertia() )^rb;
			massSum += objectB->getInvertedMass();

			float Kn = massSum + collisionPoint->mNormal*sm;
			collisionPoint->Kn = 1.0f/Kn;

			float bias=0.5f;
			collisionPoint->mBiasImpulse = bias*invDt*(collisionPoint->mDepth - 0.1f);

			vec3 t = (collisionPoint->mNormal^Vab^collisionPoint->mNormal).normalize();
			sm  = ( (ra^t)*objectA->retInvertedInertia() )^ra;
			sm += ( (rb^t)*objectB->retInvertedInertia() )^rb;

			float Kf = massSum + (t*sm);
			collisionPoint->Kf = 1.0f/Kf;

			vec3 P = collisionPoint->mNormal*collisionPoint->J + t*collisionPoint->Jf;
			
			objectA->applyImpulse(collisionPoint->mPoint, vec3(-P.x, -P.y, -P.z));
			objectB->applyImpulse(collisionPoint->mPoint, P);
		}
	}

	int iterations = 10;
	float invIter = 1.0f/(float)iterations;
	for (int i = 0; i < 10; i++)
	{
		for (phCollisionManager::CollisionsList::ValuesList::iterator it = 
			 collisionManager->mCollisions->mValues.begin(); it != collisionManager->mCollisions->mValues.end(); ++it)
		{
			phCollision* collision = *it;
		
			phObject* objectA = collision->mObjectA;
			phObject* objectB = collision->mObjectB;

			for (phCollision::CollisionPointsList::ValuesList::iterator jt = collision->mPoints->mValues.begin();
				 jt != collision->mPoints->mValues.end(); ++jt)
			{
				phCollisionPoint* collisionPoint = *jt;

				vec3 ra = collisionPoint->mPoint - objectA->getPos();
				vec3 rb = collisionPoint->mPoint - objectB->getPos();

				float Mu = 0.2f, E = 0.2f;

				vec3 Vab(0), biasVab(0);
				Vab -= objectA->getVelocity(collisionPoint->mPoint);
				biasVab -= objectA->getBiasVelocity(collisionPoint->mPoint);
				
				Vab += objectB->getVelocity(collisionPoint->mPoint);
				biasVab += objectB->getBiasVelocity(collisionPoint->mPoint);

				float J = ( Vab*collisionPoint->mNormal*-(E + 1.0f) )*collisionPoint->Kn;

				float tempJ = collisionPoint->J;
				collisionPoint->J = fmax(J + tempJ, 0.0f);
				J = collisionPoint->J - tempJ;            // J impulse


				float bias = (-(biasVab*collisionPoint->mNormal) + collisionPoint->mBiasImpulse)*collisionPoint->Kn;

				float tempBias = collisionPoint->B;
				collisionPoint->B = fmax(bias + tempBias, 0.0f);
				bias = collisionPoint->B - tempBias;      // BIAS impulse

				//logg::debugOut("J=%f, bias=%f\n", J, bias);

				Vab = vec3(0);
				objectA->applyImpulse(collisionPoint->mPoint, collisionPoint->mNormal*(-J),
					                                          collisionPoint->mNormal*(-bias));

				Vab -= objectA->getVelocity(collisionPoint->mPoint);
				
				objectB->applyImpulse(collisionPoint->mPoint, collisionPoint->mNormal*(J),
					                                          collisionPoint->mNormal*(bias));
				
				Vab += objectB->getVelocity(collisionPoint->mPoint);

				vec3 t = (collisionPoint->mNormal^Vab^collisionPoint->mNormal).normalize();

				float frt = (Vab*t)*collisionPoint->Kf;

				float frict = collisionPoint->J*Mu;
				if (frt < frict) frict = frt;
				frict = fmin(frict, collisionPoint->J*Mu);
				
				objectA->applyImpulse(collisionPoint->mPoint, t*( frict));
				objectB->applyImpulse(collisionPoint->mPoint, t*(-frict));
			}
		}
	}
}
