#ifndef VEHICLE_CREATOR_WND_H
#define VEHICLE_CREATOR_WND_H

#include <string>
#include "ui/ui_binding_values.h"

struct uiWindow;
struct uiWidgetsManager;
struct uiTextEdit;
struct uiScrollbar;
struct uiWidget;

struct phVehicleChassisComponent;
struct phVehicle;

struct cCallbackInterface;

struct VehicleCreatorWidnow
{
	struct ChassisEditContainer
	{
		uiTextEdit*  mPosX;
		uiTextEdit*  mPosY;
		uiTextEdit*  mPosZ;
					 
		uiTextEdit*  mAngleXEdit;
		uiTextEdit*  mAngleYEdit;
		uiTextEdit*  mAngleZEdit;
		uiScrollbar* mAngleXScrollbar;
		uiScrollbar* mAngleYScrollbar;
		uiScrollbar* mAngleZScrollbar;

		float        mAngleX, mAngleY, mAngleZ;
		
		uiTextEdit*  mMinPos;
		uiScrollbar* mMinPosScrollbar;
		uiTextEdit*  mMaxPos;
		uiScrollbar* mMaxPosScrollbar;

		phVehicleChassisComponent* mChassis;

	//functions
		void create(uiWidget* parentWidget, phVehicleChassisComponent* chassis, const std::string& name);

		uiBindingValues::BindValue<float>* addPropertyWithScrollbar( uiWidgetsManager* widgetsManager, std::string uiSuffix, 
			                                                         std::string caption, 
			                                                         uiTextEdit* &textEdit, 
			                                                         cCallbackInterface* callback, float minLimit, 
																	 float maxLimit, uiScrollbar* &scrollbar, 
			                                                         float& bindingValue, uiWidget* parentWidget );

		void anglesChanged();
	};

	uiWindow*         mWindow;
	uiWidgetsManager* mWidgetsManager;

	phVehicle*        mVehicle;
	
	ChassisEditContainer mLeftForwardChassis;
	ChassisEditContainer mRightForwardChassis;
	ChassisEditContainer mLeftRearChassis;
	ChassisEditContainer mRightRearChassis;

	bool              mSymmetricChanges;

//functions
	VehicleCreatorWidnow(uiWidgetsManager* widgetsManager, phVehicle* vehicle);
	~VehicleCreatorWidnow();

	void show();

	void resetParametres();

protected:
	void onSymmetricChangesChkBoxChanged();
};

#endif //VEHICLE_CREATOR_WND_H