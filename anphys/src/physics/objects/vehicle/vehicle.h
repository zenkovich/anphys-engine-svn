#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>

#include "../rigid_object.h"

struct phVehicleComponent;
struct phCollision;
struct lPolygon;
struct phCollisionPoint;

struct phCollisionGeometryVertex
{
	vec3              mLocalPos;
	vec3              mGlobalPos;

	phCollisionPoint* mCollisionPoint;

	phCollisionGeometryVertex(const vec3& pos)
	{
		mLocalPos = pos;
		mGlobalPos = pos;
		mCollisionPoint = NULL;
	}
};

struct phVehicle:public phRigidObject
{
	typedef std::vector<phVehicleComponent*> ComponentsList;
	typedef std::vector<phCollisionGeometryVertex> PointsList;
	typedef std::vector<phCollisionPoint*> CollisionPointsList;

	ComponentsList mComponents;

	PointsList   mCollisionGeometryPoints;
	CollisionPointsList mActiveCollisionPoints;
	CollisionPointsList mFreeCollisionPoints;

	lPolygon**   mPolygonsBuffer;
	unsigned int mPolygonsBufferCount;

//functions
	phVehicle();
	~phVehicle();

	void addComponent(phVehicleComponent* component);
	void removeComponent(phVehicleComponent* component);
	void removeAllComponents();

	phVehicleComponent* getComponent(const std::string& id);

	void preSolve(float dt);
	void postSolve(float dt);

	void setPolygonsBuffer(lPolygon** buffer, unsigned int count)
	{
		mPolygonsBuffer = buffer;
		mPolygonsBufferCount = count;
	}

protected:
	void updateCollisionGeometry();
	void checkCollisions();
	void solveCollisions(float dt);
};

#endif //VEHICLE_H