#include "sequential_impulses_solver.h"

#include "../util/debug/render_stuff.h"

#include "physics_scene.h"
#include "physics_object.h"
#include "collision_manager.h"
#include "collision.h"
#include "collision_point.h"
#include "collision_geometry.h"
#include "collision_geometry_part.h"

int debugDepth = 0;

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

		if (debugDepth > 0) *gLog << formatStr("solving contact: %x %x, %i points\n", objectA, objectB, collision->mPoints->mValues.size());

		for (phCollision::CollisionPointsList::ValuesList::iterator jt = collision->mPoints->mValues.begin();
			 jt != collision->mPoints->mValues.end(); ++jt)
		{
			phCollisionPoint* collisionPoint = *jt;

			if (debugDepth > 0) 
			{
				*gLog << formatStr("--normal %.2f %.2f %.2f point %.2f %.2f %.2f depth %.2f B %.2f J %.2f Jf%.2f Kn %.2f Kf %.2f",
					collisionPoint->mNormal.x, collisionPoint->mNormal.y, collisionPoint->mNormal.z,
					collisionPoint->mPoint.x, collisionPoint->mPoint.y, collisionPoint->mPoint.z,
					collisionPoint->mDepth, 
					collisionPoint->B, collisionPoint->J, collisionPoint->Jf, collisionPoint->Kn, collisionPoint->Kf);
			}

			vec3 ra = collisionPoint->mPoint - objectA->getPos();
			vec3 rb = collisionPoint->mPoint - objectB->getPos();

			vec3 Vab(0), sm(0);
			float massSum=0;
			
			Vab -= objectA->getVelocity(collisionPoint->mPoint);
			sm += ( (ra^collisionPoint->mNormal)*objectA->getInvertedInertia() )^ra;
			massSum += objectA->getInvertedMass();

			Vab += objectB->getVelocity(collisionPoint->mPoint);
			sm += ( (rb^collisionPoint->mNormal)*objectB->getInvertedInertia() )^rb;
			massSum += objectB->getInvertedMass();

			float Kn = massSum + collisionPoint->mNormal*sm;
			collisionPoint->Kn = 1.0f/Kn;

			float bias=0.5f;
			float biasDepth = collisionPoint->mDepth - 0.05f;
			collisionPoint->mSolved = biasDepth < 0.00001f;
			if (collisionPoint->mSolved) continue;

			collisionPoint->mBiasImpulse = bias/dt*biasDepth;

			vec3 t = (collisionPoint->mNormal^Vab^collisionPoint->mNormal).normalize();
			sm  = ( (ra^t)*objectA->getInvertedInertia() )^ra;
			sm += ( (rb^t)*objectB->getInvertedInertia() )^rb;

			float Kf = massSum + (t*sm);
			collisionPoint->Kf = 1.0f/Kf;

			vec3 P = collisionPoint->mNormal*collisionPoint->J + t*collisionPoint->Jf;

			if (debugDepth > 0) *gLog << formatStr("P %.2f %.2f %.2f \n", P.x, P.y, P.z);
			
			objectA->applyImpulse(collisionPoint->mPoint, vec3(-P.x, -P.y, -P.z));
			objectB->applyImpulse(collisionPoint->mPoint, P);
		}
	}

	int iterations = 10;
	float invIter = 1.0f/(float)iterations;
	for (int i = 0; i < iterations; i++)
	{
		if (debugDepth > 0) *gLog << formatStr("-----iteration %i------\n", i);
		for (phCollisionManager::CollisionsList::ValuesList::iterator it = 
			 collisionManager->mCollisions->mValues.begin(); it != collisionManager->mCollisions->mValues.end(); ++it)
		{
			phCollision* collision = *it;
		
			phObject* objectA = collision->mObjectA;
			phObject* objectB = collision->mObjectB;
			
			if (debugDepth > 0) *gLog << formatStr("solving iteration contact: %x %x, %i points\n", objectA, objectB, collision->mPoints->mValues.size());

			for (phCollision::CollisionPointsList::ValuesList::iterator jt = collision->mPoints->mValues.begin();
				 jt != collision->mPoints->mValues.end(); ++jt)
			{
				phCollisionPoint* collisionPoint = *jt;

				if (collisionPoint->mSolved) continue;

				vec3 ra = collisionPoint->mPoint - objectA->getPos();
				vec3 rb = collisionPoint->mPoint - objectB->getPos();

				float Mu = 0.4f, E = 0.1f;

				vec3 Vab(0), biasVab(0);
				Vab -= objectA->getVelocity(collisionPoint->mPoint);
				biasVab -= objectA->getBiasVelocity(collisionPoint->mPoint);
				
				Vab += objectB->getVelocity(collisionPoint->mPoint);
				biasVab += objectB->getBiasVelocity(collisionPoint->mPoint);

				float J = ( Vab*collisionPoint->mNormal*-(E + 1.0f) )*collisionPoint->Kn;

				if (debugDepth > 0) *gLog << formatStr("J %.2f ", J);

				float tempJ = collisionPoint->J;
				collisionPoint->J = fmax(J + tempJ, 0.0f);
				J = collisionPoint->J - tempJ;           // J impulse

				float bias = (-(biasVab*collisionPoint->mNormal) + collisionPoint->mBiasImpulse)*collisionPoint->Kn;
				
				if (debugDepth > 0) 
				{
					*gLog << formatStr(" bias %.2f bvab %.2f %.2f %.2f bimp %.2f  ", bias, biasVab.x, biasVab.y, biasVab.z,
						collisionPoint->mBiasImpulse);
				}

				float tempBias = collisionPoint->B;
				collisionPoint->B = fmax(bias + tempBias, 0.0f);
				bias = collisionPoint->B - tempBias;      // BIAS impulse

				//logg::debugOut("J=%f, bias=%f\n", J, bias);
				
				//getRenderStuff().addGreenArrow(collisionPoint->mPoint, collisionPoint->mPoint + collisionPoint->mNormal*(J)*0.1f);
				//getRenderStuff().addGreenArrow(collisionPoint->mPoint, collisionPoint->mPoint + collisionPoint->mNormal*(bias));

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
				
				vec3 Jf = t*( frict);
				if (debugDepth > 0) *gLog << formatStr("Jf %.2f %.2f %.2f \n", Jf.x, Jf.y, Jf.z);

				//getRenderStuff().addRedArrow(collisionPoint->mPoint, collisionPoint->mPoint + Jf*0.1f);
				
				objectA->applyImpulse(collisionPoint->mPoint, t*( frict));
				objectB->applyImpulse(collisionPoint->mPoint, t*(-frict));
			}
		}
	}
}
