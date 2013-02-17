#include "sequential_impulses_solver.h"

#include "util/debug/render_stuff.h"

#include "physics/scenes/physics_scene.h"
#include "physics/objects/physics_object.h"
#include "physics/CD/collision_manager.h"
#include "physics/CD/collision_geometry/collision.h"
#include "physics/CD/collision_geometry/collision_point.h"
#include "physics/CD/collision_geometry/collision_geometry.h"
#include "physics/CD/collision_geometry/collision_geometry_part.h"

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

/*	for (phCollisionManager::CollisionsList::ValuesList::iterator it = 
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
					collisionPoint->B, collisionPoint->J, collisionPoint->Jf1, collisionPoint->Kn, collisionPoint->Kf1);
			}

			vec3 ra = collisionPoint->mPoint - objectA->getPos();
			vec3 rb = collisionPoint->mPoint - objectB->getPos();

			vec3 Vab(0), sm(0);
			float massSum=0;
			
			Vab += objectA->getVelocity(collisionPoint->mPoint);
			sm += ( (ra^collisionPoint->mNormal)*objectA->getInvertedInertia() )^ra;
			massSum += objectA->getInvertedMass();

			Vab -= objectB->getVelocity(collisionPoint->mPoint);
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
			vec3 t2 = (vec3(0, 1, 1)^Vab^collisionPoint->mNormal).normalize();
			vec3 t3 = collisionPoint->mNormal^t2;
			
			sm  = ( (ra^t2)*objectA->getInvertedInertia() )^ra;
			sm += ( (rb^t2)*objectB->getInvertedInertia() )^rb;
			

			float Kf1 = massSum + (t2*sm);
			collisionPoint->Kf1 = 1.0f/Kf1;

			sm  = ( (ra^t3)*objectA->getInvertedInertia() )^ra;
			sm += ( (rb^t3)*objectB->getInvertedInertia() )^rb;

			float Kf2 = massSum + (t3*sm);
			collisionPoint->Kf2 = 1.0f/Kf2;

			vec3 P = collisionPoint->mNormal*collisionPoint->J - t2*collisionPoint->Jf1 - t3*collisionPoint->Jf2;

			if (debugDepth > 0) *gLog << formatStr("P %.2f %.2f %.2f \n", P.x, P.y, P.z);
			
			objectB->applyImpulse(collisionPoint->mPoint, vec3(-P.x, -P.y, -P.z));
			objectA->applyImpulse(collisionPoint->mPoint, P);
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

				float Mu = 0.8f, E = 0.4f;

				vec3 Vab(0), biasVab(0);
				Vab += objectA->getVelocity(collisionPoint->mPoint);
				biasVab += objectA->getBiasVelocity(collisionPoint->mPoint);
				
				Vab -= objectB->getVelocity(collisionPoint->mPoint);
				biasVab -= objectB->getBiasVelocity(collisionPoint->mPoint);

				float J = ( Vab*collisionPoint->mNormal*-(E + 1.0f) )*collisionPoint->Kn;

				if (debugDepth > 0) *gLog << formatStr("J %.2f ", J);

				collisionPoint->J += J;
				if (collisionPoint->J < 0.0f)
				{
					J += (0.0f - collisionPoint->J);
					collisionPoint->J = 0.0f;
				}

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

				//Vab = vec3(0);
				objectA->applyImpulse(collisionPoint->mPoint, collisionPoint->mNormal*(J),
					                                          collisionPoint->mNormal*(bias));

				//Vab -= objectA->getVelocity(collisionPoint->mPoint);
				
				objectB->applyImpulse(collisionPoint->mPoint, collisionPoint->mNormal*(-J),
					                                          collisionPoint->mNormal*(-bias));
				
				//Vab += objectB->getVelocity(collisionPoint->mPoint);
				
				vec3 t = (collisionPoint->mNormal^Vab^collisionPoint->mNormal).normalize();
				vec3 t2 = (vec3(1)^Vab^collisionPoint->mNormal).normalize();
				vec3 t3 = collisionPoint->mNormal^t2;
				
				getRenderStuff().addBlueArrow(collisionPoint->mPoint, collisionPoint->mPoint + t2);
				getRenderStuff().addBlueArrow(collisionPoint->mPoint, collisionPoint->mPoint + t3);
				getRenderStuff().addBlueArrow(collisionPoint->mPoint, collisionPoint->mPoint + collisionPoint->mNormal);
				
				float frt1 = (Vab*t2)*collisionPoint->Kf1;
				float frt2 = (Vab*t3)*collisionPoint->Kf2;
				
				float frt1temp = collisionPoint->Jf1 + frt1;
				float frt2temp = collisionPoint->Jf2 + frt2;
				float maxFrt = collisionPoint->J*Mu;
				
				*gLog << formatStr("frt1 %.3f frt2 %.3f -> ", frt1, frt2);

				if (frt1temp*frt1temp + frt2temp*frt2temp > maxFrt*maxFrt)
				{
					float invlen = 1.0f/sqrtf(frt1*frt1 + frt2*frt2);
					frt1 = frt1*collisionPoint->J*Mu*invlen - collisionPoint->Jf1;
					frt2 = frt2*collisionPoint->J*Mu*invlen - collisionPoint->Jf2;
				}
				
				collisionPoint->Jf1 += frt1;
				collisionPoint->Jf2 += frt2;
				
				*gLog << formatStr("frt1 %.3f frt2 %.3f\n", frt1, frt2);
				
				vec3 Jf = t2*frt1 + t3*frt2;

				objectA->applyImpulse(collisionPoint->mPoint, Jf);
				objectB->applyImpulse(collisionPoint->mPoint, Jf*-1.0f);
			}
		}
	}*/

	float E = 0.3f, Mu = 0.5f, biasERP = 0.8f;
	int iterations = 10;

	 //pre step
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

			float contactDepth = collisionPoint->mDepth - 0.05f;
			if (contactDepth > 0.0f)
			{
				collisionPoint->mBiasImpulse = contactDepth*invDt*biasERP;
			}
			else collisionPoint->mBiasImpulse = 0.0f;

			getRenderStuff().addRedArrow(collisionPoint->mPoint, collisionPoint->mPoint + collisionPoint->mNormal*0.5f);

			collisionPoint->t1 = collisionPoint->mNormal^vec3(0, 0, 1);
			if (collisionPoint->t1*collisionPoint->t1 < 0.0001f)
				collisionPoint->t1 = collisionPoint->mNormal^vec3(0, 1, 0);

			collisionPoint->t2 = collisionPoint->mNormal^collisionPoint->t1;
			
			getRenderStuff().addBlueArrow(collisionPoint->mPoint, collisionPoint->mPoint + collisionPoint->t1*0.5f);
			getRenderStuff().addGreenArrow(collisionPoint->mPoint, collisionPoint->mPoint + collisionPoint->t2*0.5f);

			vec3 imp = collisionPoint->mNormal*collisionPoint->J + collisionPoint->t1*collisionPoint->Jf1 + 
				                                                   collisionPoint->t2*collisionPoint->Jf2;
			
			objectA->applyImpulse(collisionPoint->mPoint, imp);
			objectB->applyImpulse(collisionPoint->mPoint, imp*-1.0f);

			vec3 ra = collisionPoint->mPoint - objectA->getPos();
			vec3 rb = collisionPoint->mPoint - objectB->getPos();
			
			vec3 n1 = collisionPoint->mNormal;            collisionPoint->n1 = n1;
			vec3 w1 = collisionPoint->mNormal^ra;         collisionPoint->w1 = w1;
			vec3 n2 = collisionPoint->mNormal*-1.0f;      collisionPoint->n2 = n2;
			vec3 w2 = (collisionPoint->mNormal^rb)*-1.0f; collisionPoint->w2 = w2;

			float b = n1*n1*objectA->getInvertedMass() + w1*(w1*objectA->getInvertedInertia()) + 
						n2*n2*objectB->getInvertedMass() + w2*(w2*objectB->getInvertedInertia());
			collisionPoint->B = 1.0f/b;

				
			vec3 f1n1 = collisionPoint->t1;            collisionPoint->f1n1 = f1n1;
			vec3 f1w1 = collisionPoint->t1^ra;         collisionPoint->f1w1 = f1w1;
			vec3 f1n2 = collisionPoint->t1*-1.0f;      collisionPoint->f1n2 = f1n2;
			vec3 f1w2 = (collisionPoint->t1^rb)*-1.0f; collisionPoint->f1w2 = f1w2;
				
			float f1b = f1n1*f1n1*objectA->getInvertedMass() + f1w1*(f1w1*objectA->getInvertedInertia()) + 
						f1n2*f1n2*objectB->getInvertedMass() + f1w2*(f1w2*objectB->getInvertedInertia());
			collisionPoint->Bf1 = 1.0f/f1b;
				
			vec3 f2n1 = collisionPoint->t2;            collisionPoint->f2n1 = f2n1;
			vec3 f2w1 = collisionPoint->t2^ra;         collisionPoint->f2w1 = f2w1;
			vec3 f2n2 = collisionPoint->t2*-1.0f;      collisionPoint->f2n2 = f2n2;
			vec3 f2w2 = (collisionPoint->t2^rb)*-1.0f; collisionPoint->f2w2 = f2w2;

			float f2b = f2n1*f2n1*objectA->getInvertedMass() + f2w1*(f2w1*objectA->getInvertedInertia()) + 
						f2n2*f2n2*objectB->getInvertedMass() + f2w2*(f2w2*objectB->getInvertedInertia());
			collisionPoint->Bf2 = 1.0f/f2b;
			
			/*float contactDepth = collisionPoint->mDepth - 0.01f;
			if (contactDepth > 0.0f)
			{
				float biasA = n1*objectA->getBiasVelocity() + n2*objectB->getBiasVelocity() + 
							  w1*objectA->getBiasAngularVelocity() + w2*objectB->getBiasAngularVelocity();


				float biasLambda = -(biasA - contactDepth*invDt*biasERP)/b;

				*gLog << formatStr("biasLambda = %.3f\n", biasLambda);

				if (biasLambda > 0.0f)
				{					
					objectA->applyBiasImpulse(collisionPoint->mPoint, collisionPoint->mNormal*biasLambda);
					objectB->applyBiasImpulse(collisionPoint->mPoint, collisionPoint->mNormal*-biasLambda);
				}
			}*/
		}
	}

	for (int i = 0; i < iterations; i++)
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
			
				vec3 n1 = collisionPoint->n1;
				vec3 w1 = collisionPoint->w1;
				vec3 n2 = collisionPoint->n2;
				vec3 w2 = collisionPoint->w2;

				float a = n1*objectA->getVelocity() + n2*objectB->getVelocity() + 
						  w1*objectA->getAngularVelocity() + w2*objectB->getAngularVelocity();
				float b =collisionPoint->B;

				float lambda = -a*(E + 1.0f)*b;

				collisionPoint->J += lambda;
				if(collisionPoint->J < 0.0f)
				{
				  lambda += (0.0f - collisionPoint->J);
				  collisionPoint->J = 0.0f;
				}

				objectA->applyImpulse(collisionPoint->mPoint, collisionPoint->mNormal*lambda);
				objectB->applyImpulse(collisionPoint->mPoint, collisionPoint->mNormal*-lambda);
				
				vec3 f1n1 = collisionPoint->f1n1;
				vec3 f1w1 = collisionPoint->f1w1;
				vec3 f1n2 = collisionPoint->f1n2;
				vec3 f1w2 = collisionPoint->f1w2;
				
				float f1a = f1n1*objectA->getVelocity() + f1n2*objectB->getVelocity() + 
						    f1w1*objectA->getAngularVelocity() + f1w2*objectB->getAngularVelocity();
				float f1b = collisionPoint->Bf1;

				float f1lambda = -f1a*f1b;
				
				vec3 f2n1 = collisionPoint->f2n1;
				vec3 f2w1 = collisionPoint->f2w1;
				vec3 f2n2 = collisionPoint->f2n2;
				vec3 f2w2 = collisionPoint->f2w2;
				
				float f2a = f2n1*objectA->getVelocity() + f2n2*objectB->getVelocity() + 
						    f2w1*objectA->getAngularVelocity() + f2w2*objectB->getAngularVelocity();
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

				objectA->applyImpulse(collisionPoint->mPoint, Jf);
				objectB->applyImpulse(collisionPoint->mPoint, Jf*-1.0f);

				/*if (collisionPoint->mBiasImpulse > 0.0f)
				{*/
					float biasA = n1*objectA->getBiasVelocity() + n2*objectB->getBiasVelocity() + 
							      w1*objectA->getBiasAngularVelocity() + w2*objectB->getBiasAngularVelocity();

					float biasLambda = -(biasA - collisionPoint->mBiasImpulse)*b;

					//*gLog << formatStr("biasA %.3f bias imp %.3f lambda %.3f\n", biasA, collisionPoint->mBiasImpulse, biasLambda);

					getRenderStuff().addBlueArrow(collisionPoint->mPoint, collisionPoint->mPoint + 
						collisionPoint->mNormal*biasLambda*dt/b);
					
					objectA->applyBiasImpulse(collisionPoint->mPoint, collisionPoint->mNormal*biasLambda);
					objectB->applyBiasImpulse(collisionPoint->mPoint, collisionPoint->mNormal*-biasLambda);
				//}
			}
		}
	}
}
