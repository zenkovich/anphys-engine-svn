#ifndef VEHICLE_CREATOR_WND_H
#define VEHICLE_CREATOR_WND_H

#include <string>
#include "ui/ui_binding_values.h"

struct uiWindow;
struct uiWidgetsManager;
struct uiTextEdit;
struct uiScrollbar;
struct uiWidget;

namespace physics
{
	struct Vehicle;
	struct VehicleChassis;
}

struct cCallbackInterface;

struct VehicleCreatorWidnow
{
	enum ChassisId { CH_LEFT_FWD = 0, CH_RIGHT_FWD, CH_LEFT_REAR, CH_RIGHT_REAR };
	enum ChassisPropId { PID_POSX = 0, PID_POSY, PID_POSZ,
	                     PID_ANGLEX, PID_ANGLEY, PID_ANGLEZ,
	                     PID_MINPOS, PID_MAXPOS,
	                     PID_SPRING_FORCE, PID_SHOCK_FORCE };

	struct ChassisEditContainer
	{
		VehicleCreatorWidnow*    mVehicleCreator;
		ChassisId                mChassisId;

		uiTextEdit*              mPosX;
		uiTextEdit*              mPosY;
		uiTextEdit*              mPosZ;
					             
		uiTextEdit*              mAngleXEdit;
		uiTextEdit*              mAngleYEdit;
		uiTextEdit*              mAngleZEdit;
		uiScrollbar*             mAngleXScrollbar;
		uiScrollbar*             mAngleYScrollbar;
		uiScrollbar*             mAngleZScrollbar;
					             
		float                    mAngleX, mAngleY, mAngleZ;
					             
		uiTextEdit*              mMinPos;
		uiScrollbar*             mMinPosScrollbar;
		uiTextEdit*              mMaxPos;
		uiScrollbar*             mMaxPosScrollbar;
					             
		uiTextEdit*              mSpringForce;
		uiScrollbar*             mSpringForceScrollbar;
		uiTextEdit*              mGasShockForce;
		uiScrollbar*             mGasShockForceScrollbar;

		physics::VehicleChassis* mChassis;

	//functions
		void create(uiWidget* parentWidget, ChassisId id, VehicleCreatorWidnow* owner, physics::VehicleChassis* chassis, 
			        const std::string& name);

		uiBindingValues::BindValue<float>* addPropertyWithScrollbar( uiWidgetsManager* widgetsManager, std::string uiSuffix, 
			                                                         std::string caption, 
			                                                         uiTextEdit* &textEdit, 
			                                                         cCallbackInterface* callback, float minLimit, 
																	 float maxLimit, uiScrollbar* &scrollbar, 
			                                                         float& bindingValue, uiWidget* parentWidget );

		void anglesChanged();
	};

	uiWindow*            mWindow;
	uiWidgetsManager*    mWidgetsManager;

	physics::Vehicle*    mVehicle;
	
	ChassisEditContainer mLeftForwardChassis;
	ChassisEditContainer mRightForwardChassis;
	ChassisEditContainer mLeftRearChassis;
	ChassisEditContainer mRightRearChassis;

	bool                 mSymmetricChanges;

//functions
	VehicleCreatorWidnow(uiWidgetsManager* widgetsManager, physics::Vehicle* vehicle);
	~VehicleCreatorWidnow();

	void show();

	void resetParametres();

protected:
	void onSymmetricChangesChkBoxChanged();

	void onMassEditChanged();

	void onInertiaChanged();

	void onChassisPropertyChanged(ChassisId chassidId, ChassisPropId propId);
};

#endif //VEHICLE_CREATOR_WND_H