#ifndef VEHICLE_CHASSIS_SPRING_H
#define VEHICLE_CHASSIS_SPRING_H

struct phVehicleChassisSpring
{
	float mForce;

	float getForce(float diff) { return diff*mForce; }
};

#endif //VEHICLE_CHASSIS_SPRING_H