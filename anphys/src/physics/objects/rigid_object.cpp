#include "rigid_object.h"

#include "../CD/collision_geometry/collision_geometry.h"
#include "../CD/collision_geometry/collision.h"
#include "../CD/collision_geometry/collision_point.h"

phRigidObject::phRigidObject():phObject(), mSleeping(sleep_stage_sleep)
{
	mCollisionGeometry = new phCollisionGeometry(this);

	mType = rigid_physics_object;
}

phRigidObject::phRigidObject(phScene* scene):phObject(scene), mSleeping(sleep_stage_sleep)
{
	mCollisionGeometry = new phCollisionGeometry(this);

	mType = rigid_physics_object;
}

phRigidObject::phRigidObject(vec3 position, mat3x3 orientation, float mass, mat3x3 inertia, 
							 phCollisionGeometry* collisionGeometry):phObject(),  
							 mSleeping(sleep_stage_sleep), mPosition(position), mOrient(orientation), mMass(mass),
							 mInertia(inertia), mCollisionGeometry(collisionGeometry)
{
	mCollisionGeometry->mObject = this;

	mType = rigid_physics_object;

	mInvMass = 1.0f/mMass;
	mInvInertia = mInertia.inverse();
	mWorldInertia = mInertia*mOrient;
	mInvWorldInertia = mWorldInertia.inverse();

	mOrientQuat = matrix2quat(mOrient);
}

phRigidObject::~phRigidObject()
{
	safe_release(mCollisionGeometry);
}

void phRigidObject::update(float dt)
{
	preSolve(dt);
	mCollisionGeometry->update(dt);
	postSolve(dt);
}

void phRigidObject::preSolve(float dt)
{
	mVelocity += mForce;
	mAngularVelocity += mTorque;

	mForce = mTorque = vec3(0);

	mCollisionGeometry->preUpdate(dt);
}

void phRigidObject::postSolve(float dt)
{
	mPosition += (mVelocity + mBiasVelocity)*dt;

	mOrientQuat -= mOrientQuat*((mAngularVelocity + mBiasAngularVelocity)*dt)*0.5f;
	mOrientQuat = mOrientQuat.normalize();
	mOrient = quat2matrix(mOrientQuat);
	//mOrient = mOrient*((mAngularVelocity + mBiasAngularVelocity)*dt);

	mBiasVelocity = mBiasAngularVelocity = vec3(0);

	mInvWorldInertia = (mOrient)*mInvInertia*(mOrient.transpose());
	mInvWorldInertia = mInvInertia;

	mCollisionGeometry->postUpdate(dt);

	/**gLog << formatStr("rigid object %x: pos %.2f %.2f %.2f, orient (%.2f %.2f %.2f %.2f), vel %.2f %.2f %.2f, avel %.2f %.2f %.2f\n",
		this, mPosition.x, mPosition.y, mPosition.z, mOrientQuat.v.x, mOrientQuat.v.y, mOrientQuat.v.z, mOrientQuat.w,
		mVelocity.x, mVelocity.y, mVelocity.z, mAngularVelocity.x, mAngularVelocity.y, mAngularVelocity.z);*/
}

void phRigidObject::setPos(vec3 pos)
{
	mPosition = pos;
}

void phRigidObject::setOrient(mat3x3 orient)
{
	mOrient = orient;
}

bool phRigidObject::isSleep()
{
	return mSleeping == sleep_stage_sleep;
}

void phRigidObject::setSleep(bool flag)
{
	if (flag) mSleeping = sleep_stage_sleep;
}

void phRigidObject::applyImpulse(vec3& point, vec3& impulse)
{
	mVelocity += impulse*mInvMass;
	mAngularVelocity += (impulse^(point - mPosition))*mInvWorldInertia;
}
	
void phRigidObject::applyImpulse(phCollisionPoint* collisionPoint)
{
	vec3 impulse = collisionPoint->mImpulse;
	vec3 biasImpulse = collisionPoint->mBiasImpulse;
	if (this == collisionPoint->mCollision->mObjectB) 
	{
		impulse *= -1.0f;
		biasImpulse *= -1.0f;
	}

	vec3 r = collisionPoint->mPoint - mPosition;

	mVelocity += impulse*mInvMass;
	mAngularVelocity += (impulse^r)*mInvWorldInertia;

	mBiasVelocity += biasImpulse*mInvMass;
	mBiasAngularVelocity += (biasImpulse^r)*mInvWorldInertia;
}

void phRigidObject::applyImpulse( vec3& point, vec3& impulse, vec3& biasImpulse )
{	
	vec3 r = point - mPosition;

	mVelocity += impulse*mInvMass;
	mAngularVelocity += (impulse^r)*mInvWorldInertia;

	mBiasVelocity += biasImpulse*mInvMass;
	mBiasAngularVelocity += (biasImpulse^r)*mInvWorldInertia;
}

void phRigidObject::applyBiasImpulse( vec3& point, vec3& impulse )
{
	mBiasVelocity += impulse*mInvMass;
	mBiasAngularVelocity += (impulse^(point - mPosition))*mInvWorldInertia;
}

void phRigidObject::addForce(vec3& force)
{
	mForce += force;
}

void phRigidObject::addTorque(vec3& torque)
{
	mTorque += torque;
}

phCollision* phRigidObject::checkCollision(phObject* object, phCollision* collision)
{
	phCollisionGeometry* collisionGeomB = object->getCollisionGeometry();
	if (!collisionGeomB) return NULL;

	return mCollisionGeometry->checkCollision(collisionGeomB, collision);
}

phCollisionGeometry* phRigidObject::getCollisionGeometry()
{
	return mCollisionGeometry;
}