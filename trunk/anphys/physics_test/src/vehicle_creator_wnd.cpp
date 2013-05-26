#include "vehicle_creator_wnd.h"

#include "engine/engine_incl.h"

#include "physics/objects/vehicle/vehicle.h"
#include "physics/objects/vehicle/chassis_vehicle_component.h"
#include "physics/objects/vehicle/vehicle_chassis_spring.h"
#include "physics/objects/vehicle/vehicle_chassis_gas_shock.h"
#include "util/debug/render_stuff.h"

VehicleCreatorWidnow::VehicleCreatorWidnow( uiWidgetsManager* widgetsManager, phVehicle* vehicle ):
	mWidgetsManager(widgetsManager), mVehicle(vehicle), mSymmetricChanges(true)
{
	mWindow = uiSimpleStuff::createWindow(mWidgetsManager, "VehicleCreatorWnd", vec2(200, 0), vec2(300, 600), 
		"Vehicle settings");

	uiSimpleStuff::createSizeEffect(mWindow);

	uiVertLayoutWidget* verLayoutWidget = new uiVertLayoutWidget(mWidgetsManager, "verLayout");
	verLayoutWidget->setPosition(vec2(5, 5));
	verLayoutWidget->mWidgetsDistance = 5.0f;

	vec2 labelSize(100, 20);

	uiButton* resetParametresButton = uiSimpleStuff::createButton(mWidgetsManager, vec2(0, 0), vec2(200, 20), "RestartBtn",
		"Reset parametres", new cCallback<VehicleCreatorWidnow>(this, &VehicleCreatorWidnow::resetParametres));
	verLayoutWidget->addChild((uiWidget*)resetParametresButton);

//position
	verLayoutWidget->addChild(uiSimpleStuff::createLabel(mWidgetsManager, vec2(0, 0), vec2(100, 20), "PosLabel", "Position")
		                      ->setHorAlign(uiLabel::AL_LEFT));

	uiHorLayoutWidget* positionHorLayout = new uiHorLayoutWidget(mWidgetsManager, "PosLayout");
	positionHorLayout->mWidgetsDistance = 5;

	uiTextEdit* posXEdit = uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(0, 0), vec2(50, 22), "xEdit");
	posXEdit->bindValue(&mVehicle->mPosition.x);

	uiTextEdit* posYEdit = uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(0, 0), vec2(50, 22), "yEdit");
	posYEdit->bindValue(&mVehicle->mPosition.y);

	uiTextEdit* posZEdit = uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(0, 0), vec2(50, 22), "zEdit");
	posZEdit->bindValue(&mVehicle->mPosition.z);
	
	positionHorLayout->addChild(posXEdit);
	positionHorLayout->addChild(posYEdit);
	positionHorLayout->addChild(posZEdit);
	
	verLayoutWidget->addChild(positionHorLayout);

//mass
	verLayoutWidget->addChild(uiSimpleStuff::createLabel(mWidgetsManager, vec2(0, 0), vec2(100, 20), "MassLabel", "Mass")
		                      ->setHorAlign(uiLabel::AL_LEFT));

	uiTextEdit* massEdit = uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(0, 0), vec2(50, 22), "MassEdit");
	massEdit->bindValue(&mVehicle->mMass)
		->setChangeValueCallback(new cCallback<VehicleCreatorWidnow>(this, &VehicleCreatorWidnow::onMassEditChanged));

	verLayoutWidget->addChild(massEdit);
	
//inertia
	verLayoutWidget->addChild(uiSimpleStuff::createLabel(mWidgetsManager, vec2(0, 0), vec2(100, 20), "InertiaLabel", "Inertia")
		                      ->setHorAlign(uiLabel::AL_LEFT));

	uiHorLayoutWidget* inertiaHorLayout = new uiHorLayoutWidget(mWidgetsManager, "InertiaLayout");
	inertiaHorLayout->mWidgetsDistance = 5;

	uiTextEdit* inertiaXEdit = uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(0, 0), vec2(50, 22), "xEdit");
	inertiaXEdit->bindValue(&mVehicle->mInertia.m[0][0])
		->setChangeValueCallback(new cCallback<VehicleCreatorWidnow>(this, &VehicleCreatorWidnow::onInertiaChanged));
	
	uiTextEdit* inertiaYEdit = uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(0, 0), vec2(50, 22), "yEdit");
	inertiaYEdit->bindValue(&mVehicle->mInertia.m[1][1])
		->setChangeValueCallback(new cCallback<VehicleCreatorWidnow>(this, &VehicleCreatorWidnow::onInertiaChanged));

	uiTextEdit* inertiaZEdit = uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(0, 0), vec2(50, 22), "zEdit");
	inertiaZEdit->bindValue(&mVehicle->mInertia.m[2][2])
		->setChangeValueCallback(new cCallback<VehicleCreatorWidnow>(this, &VehicleCreatorWidnow::onInertiaChanged));
	
	inertiaHorLayout->addChild(inertiaXEdit);
	inertiaHorLayout->addChild(inertiaYEdit);
	inertiaHorLayout->addChild(inertiaZEdit);
	
	verLayoutWidget->addChild(inertiaHorLayout);

//symmetric changes checkbox
	uiCheckBox* symmetricChangesCheckbox = uiSimpleStuff::createCheckbox(mWidgetsManager, "SymmetricChanges", vec2(0, 0),
		"Symmetric changes", mSymmetricChanges);
	symmetricChangesCheckbox->bindValue(&mSymmetricChanges)
		->setChangeValueCallback(new cCallback<VehicleCreatorWidnow>(this, &VehicleCreatorWidnow::onSymmetricChangesChkBoxChanged));

	verLayoutWidget->addChild(symmetricChangesCheckbox);

//chassis
	mLeftForwardChassis.create(verLayoutWidget, CH_LEFT_FWD, this,
		static_cast<phVehicleChassisComponent*>(mVehicle->getComponent("forwardLeftChassis")), "Forward Left Chassis");
	
	mRightForwardChassis.create(verLayoutWidget, CH_RIGHT_FWD, this, 
		static_cast<phVehicleChassisComponent*>(mVehicle->getComponent("forwardRightChassis")), "Forward Right Chassis");
	
	mLeftRearChassis.create(verLayoutWidget, CH_LEFT_REAR, this, 
		static_cast<phVehicleChassisComponent*>(mVehicle->getComponent("rearLeftChassis")), "Rear Left Chassis");
	
	mRightRearChassis.create(verLayoutWidget, CH_RIGHT_REAR, this,
		static_cast<phVehicleChassisComponent*>(mVehicle->getComponent("rearRightChassis")), "Rear Right Chassis");

	uiWidget* dummyWidget = new uiWidget(mWidgetsManager, "dummy");
	dummyWidget->setSize(vec2(10, 10));

	verLayoutWidget->addChild(dummyWidget);

	mWindow->addChild(verLayoutWidget);

	mWidgetsManager->addWidget(mWindow);

	resetParametres();
}

VehicleCreatorWidnow::~VehicleCreatorWidnow()
{
	mWidgetsManager->removeWidget(mWindow);
}

void VehicleCreatorWidnow::show()
{
	mWindow->show();
}

void VehicleCreatorWidnow::resetParametres()
{
	mVehicle->mPosition = vec3(0, 2, 0);
	mVehicle->setOrient(nullMatr());
	mVehicle->mOrientQuat = matrix2quat(mVehicle->mOrient);
	mVehicle->mVelocity = vec3(0, 0, 2);
	mVehicle->mAngularVelocity = vec3(0, 0, 0);
	mVehicle->mMass = 1800;
	mVehicle->mInvMass = 1.0f/mVehicle->mMass;
	mVehicle->mInertia = getBoxInertia(mVehicle->mMass, vec3(1.8f, 1.6f, 3.7f));
	mVehicle->mInvInertia = mVehicle->mInertia.inverse();
	mVehicle->mWorldInertia = mVehicle->mInertia;
	mVehicle->mInvWorldInertia = mVehicle->mInvInertia;
}

void VehicleCreatorWidnow::onSymmetricChangesChkBoxChanged()
{

}

void VehicleCreatorWidnow::onMassEditChanged()
{
	mVehicle->mInvMass = 1.0f/mVehicle->mMass;
}

void VehicleCreatorWidnow::onInertiaChanged()
{
	mVehicle->mInvInertia = mVehicle->mInertia.inverse();
}

void VehicleCreatorWidnow::onChassisPropertyChanged( ChassisId chassidId, ChassisPropId propId )
{
	if (!mSymmetricChanges)
	{		
		getRenderStuff().reset();	
		mVehicle->update(0.01f);
		return;
	}

	std::string chassisIdNames[] = { "CH_LEFT_FWD", "CH_RIGHT_FWD", "CH_LEFT_REAR", "CH_RIGHT_REAR" };
	std::string propIdNames[] = { "PID_POSX", "PID_POSY", "PID_POSZ",
	                     "PID_ANGLEX", "PID_ANGLEY", "PID_ANGLEZ",
	                     "PID_MINPOS", "PID_MAXPOS",
	                     "PID_SPRING_FORCE", "PID_SHOCK_FORCE" };

	gLog->fout(1, "changed %s %s\n", chassisIdNames[chassidId].c_str(), propIdNames[propId].c_str());
	
	ChassisEditContainer* containerA;
	ChassisEditContainer* containerB;

	if (chassidId == CH_LEFT_FWD)
	{
		containerA = &mLeftForwardChassis;
		containerB = &mRightForwardChassis;
	}
	else if (chassidId == CH_RIGHT_FWD)
	{
		containerB = &mLeftForwardChassis;
		containerA = &mRightForwardChassis;
	}
	else if (chassidId == CH_LEFT_REAR)
	{
		containerA = &mLeftRearChassis;
		containerB = &mRightRearChassis;
	}
	else if (chassidId == CH_RIGHT_REAR)
	{
		containerB = &mLeftRearChassis;
		containerA = &mRightRearChassis;
	}

	if (propId == PID_POSX)
		containerB->mChassis->mLocalPosition.x = -containerA->mChassis->mLocalPosition.x;

	if (propId == PID_POSY)
		containerB->mChassis->mLocalPosition.y = containerA->mChassis->mLocalPosition.y;

	if (propId == PID_POSZ)
		containerB->mChassis->mLocalPosition.z = containerA->mChassis->mLocalPosition.z;

	if (propId == PID_ANGLEX)
		containerB->mAngleX = containerA->mAngleX;

	if (propId == PID_ANGLEY)
		containerB->mAngleY = containerA->mAngleY;

	if (propId == PID_ANGLEZ)
		containerB->mAngleZ = -containerA->mAngleZ;

	if (propId == PID_MINPOS)
		containerB->mChassis->mMinPosition = containerA->mChassis->mMinPosition;

	if (propId == PID_MAXPOS)
		containerB->mChassis->mMaxPosition = containerA->mChassis->mMaxPosition;

	if (propId == PID_SPRING_FORCE)
		containerB->mChassis->mSpringForce = containerA->mChassis->mSpringForce;

	if (propId == PID_SHOCK_FORCE)
		containerB->mChassis->mGasShockForce = containerA->mChassis->mGasShockForce;
	
	getRenderStuff().reset();	
	mVehicle->update(0.01f);
}

void VehicleCreatorWidnow::ChassisEditContainer::create( uiWidget* parentWidget, ChassisId id, VehicleCreatorWidnow* owner, 
	phVehicleChassisComponent* chassis, const std::string& name )
{
	mAngleX = mAngleY = mAngleZ = 0;

	mChassisId = id;
	mVehicleCreator = owner;

	mAngleX = mAngleY = mAngleZ = 0;

	mChassis = chassis;

	uiWidgetsManager* widgetsManager = parentWidget->mWidgetsManager;

	uiLabel* mainCaption = uiSimpleStuff::createLabel(widgetsManager, vec2(0, 0), vec2(100, 20), 
		name + "MainCaption", name);
	mainCaption->setHorAlign(uiLabel::AL_LEFT);

	parentWidget->addChild(mainCaption);

//position
	uiLabel* posCaption = uiSimpleStuff::createLabel(widgetsManager, vec2(0, 0), vec2(100, 20), name + "PosCaption", "Position");
	posCaption->setHorAlign(uiLabel::AL_LEFT);

	parentWidget->addChild(posCaption);

	uiHorLayoutWidget* posHorLayout = new uiHorLayoutWidget(widgetsManager, name + "PosHorLayout");
	
	mPosX = uiSimpleStuff::createTextEdit(widgetsManager, vec2(0, 0), vec2(50, 22), "xEdit");
	mPosX->bindValue(&chassis->mLocalPosition.x)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_POSX));

	mPosY = uiSimpleStuff::createTextEdit(widgetsManager, vec2(0, 0), vec2(50, 22), "yEdit");
	mPosY->bindValue(&chassis->mLocalPosition.y)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_POSY));

	mPosZ = uiSimpleStuff::createTextEdit(widgetsManager, vec2(0, 0), vec2(50, 22), "zEdit");
	mPosZ->bindValue(&chassis->mLocalPosition.z)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_POSZ));
	
	posHorLayout->addChild(mPosX);
	posHorLayout->addChild(mPosY);
	posHorLayout->addChild(mPosZ);

	parentWidget->addChild(posHorLayout);

//angles
	uiLabel* anglesCaption = uiSimpleStuff::createLabel(widgetsManager, vec2(0, 0), vec2(100, 20), name + "AnglesCaption", "Angles");
	anglesCaption->setHorAlign(uiLabel::AL_LEFT);

	parentWidget->addChild(anglesCaption);

	addPropertyWithScrollbar(widgetsManager, "AngleX", "X", mAngleXEdit, 
		new cCallback<ChassisEditContainer>(this, &ChassisEditContainer::anglesChanged), -30.0f, 30.0f, mAngleXScrollbar, 
		mAngleX, parentWidget)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_ANGLEX));

	addPropertyWithScrollbar(widgetsManager, "AngleY", "Y", mAngleYEdit, 
		new cCallback<ChassisEditContainer>(this, &ChassisEditContainer::anglesChanged), -30.0f, 30.0f, mAngleYScrollbar, 
		mAngleY, parentWidget)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_ANGLEY));

	addPropertyWithScrollbar(widgetsManager, "AngleZ", "Z", mAngleZEdit, 
		new cCallback<ChassisEditContainer>(this, &ChassisEditContainer::anglesChanged), -30.0f, 30.0f, mAngleZScrollbar, 
		mAngleZ, parentWidget)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_ANGLEZ));
	
//angles
	uiLabel* wposCaption = uiSimpleStuff::createLabel(widgetsManager, vec2(0, 0), vec2(100, 20), name + "WPosCaption", "Wheel Pos");
	wposCaption->setHorAlign(uiLabel::AL_LEFT);

	parentWidget->addChild(wposCaption);
	
	addPropertyWithScrollbar(widgetsManager, "MinPos", "min", mMinPos, NULL, -1.0f, 1.0f, mMinPosScrollbar, 
		chassis->mMinPosition, parentWidget)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_MINPOS));
	
	addPropertyWithScrollbar(widgetsManager, "MaxPos", "max", mMaxPos, NULL, -1.0f, 1.0f, mMaxPosScrollbar, 
		chassis->mMaxPosition, parentWidget)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_MAXPOS));

//spring
	addPropertyWithScrollbar(widgetsManager, "Spring", "Spring", mSpringForce, NULL, 0, 200000.0f, mSpringForceScrollbar, 
		chassis->mSpringForce, parentWidget)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_SPRING_FORCE));

	addPropertyWithScrollbar(widgetsManager, "Shock", "Shock", mGasShockForce, NULL, 0, 20000.0f, mGasShockForceScrollbar, 
		chassis->mGasShockForce, parentWidget)->
		setChangeValueCallback(new cCallback2Param<ChassisId, ChassisPropId, VehicleCreatorWidnow>(
			mVehicleCreator, &VehicleCreatorWidnow::onChassisPropertyChanged, mChassisId, PID_SHOCK_FORCE));
}

void VehicleCreatorWidnow::ChassisEditContainer::anglesChanged()
{
	mChassis->mInitialLocalAxis = RotatedMatrix(rad(mAngleX), rad(mAngleY), rad(mAngleZ));
}

uiBindingValues::BindValue<float>* VehicleCreatorWidnow::ChassisEditContainer::addPropertyWithScrollbar( 
	uiWidgetsManager* widgetsManager, 
	std::string uiSuffix, std::string caption, uiTextEdit* &textEdit, 
	cCallbackInterface* callback, float minLimit, float maxLimit, uiScrollbar* &scrollbar, float& bindingValue, 
	uiWidget* parentWidget )
{
	uiHorLayoutWidget* propHorLayout = new uiHorLayoutWidget(widgetsManager, uiSuffix + "HorLayout");

	uiLabel* captionLabel = uiSimpleStuff::createLabel(widgetsManager, vec2(0, 0), vec2(40, 20), "Caption", caption);
	captionLabel->setHorAlign(uiLabel::AL_LEFT);

	textEdit = uiSimpleStuff::createTextEdit(widgetsManager, vec2(0, 0), vec2(50, 22), "Edit");
	uiBindingValues::BindValue<float>* bindValue = textEdit->bindValue(&bindingValue);
	if (callback)
		bindValue->setChangeValueCallback(callback);
	bindValue->setLimitations(minLimit, maxLimit);

	scrollbar = uiSimpleStuff::createScrollbar(widgetsManager, vec2(0, 5), vec2(100, 10), "Scrollbar", 
		uiScrollbar::ST_HORISONTAL, minLimit, maxLimit);
	uiBindingValues::BindValue<float>* bindValue2 = scrollbar->bindValue(&bindingValue);

	propHorLayout->addChild(captionLabel);
	propHorLayout->addChild(textEdit);
	propHorLayout->addChild(scrollbar);

	parentWidget->addChild(propHorLayout);

	return bindValue2;
}
