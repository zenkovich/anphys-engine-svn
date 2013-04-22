#ifndef VEHICLE_CHASSIS_GAS_SHOCK_H
#define VEHICLE_CHASSIS_GAS_SHOCK_H

struct phVehicleChassisGasShock
{
	float mForce;

	float getForce(float velocity) { return mForce*velocity; }
};

#endif //VEHICLE_CHASSIS_GAS_SHOCK_H