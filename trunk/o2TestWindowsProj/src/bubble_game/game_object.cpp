#include "game_object.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(IGameObject);

IGameObject::IGameObject(const vec2f& position /*= vec2f()*/, float rotation /*= 0*/):
	mPosition(position), mRotation(rotation)
{
	initializeProperties();
}

IGameObject::~IGameObject()
{
}

void IGameObject::setPosition(const vec2f& position)
{
	mPosition = position;
}

vec2f IGameObject::getPosition() const
{
	return mPosition;
}

void IGameObject::setRotation(float rotation)
{
	mRotation = rotation;
}

float IGameObject::getRotation() const
{
	return mRotation;
}

vec2f IGameObject::transformLocalToWorld(const vec2f& pointLocal)
{
	vec2f vx = vec2f::rotated(deg2rad(mRotation));
	vec2f vy(-vx.y, vx.x);

	return vx*pointLocal.x + vy*pointLocal.y + mPosition;
}

void IGameObject::setActive( bool active )
{
	if (mActive == active)
		return;

	mActive = active;
	if (mActive)
		onActivate();
	else
		onDeactivate();
}

bool IGameObject::isActive() const
{
	return mActive;
}

void IGameObject::initializeProperties()
{
	REG_PROPERTY                (IGameObject, position, setPosition, getPosition);
	REG_PROPERTY_SETTER_NONCONST(IGameObject, rotation, setRotation, getRotation);
	REG_PROPERTY_SETTER_NONCONST(IGameObject, active, setActive, isActive);
}

SERIALIZE_METHOD_IMPL(IGameObject)
{
	SERIALIZE_ID(mPosition, "position");
	SERIALIZE_ID(mRotation, "rotation");
	SERIALIZE_ID(mActive,   "active");

	return true;
}

void IGameObject::onDeserialized()
{
	mActive = !mActive;
	setActive(!mActive);
}

CLOSE_O2_NAMESPACE