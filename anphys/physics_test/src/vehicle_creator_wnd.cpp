#include "vehicle_creator_wnd.h"

#include "engine/engine_incl.h"

#include "physics/objects/vehicle/vehicle.h"
#include "physics/objects/vehicle/chassis_vehicle_component.h"

VehicleCreatorWidnow::VehicleCreatorWidnow( uiWidgetsManager* widgetsManager, phVehicle* vehicle ):
	mWidgetsManager(widgetsManager), mVehicle(vehicle), mSymmetricChanges(true)
{
	mWindow = uiSimpleStuff::createWindow(mWidgetsManager, "VehicleCreatorWnd", vec2(200, 0), vec2(300, 300), 
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

//symmetric changes checkbox
	uiCheckBox* symmetricChangesCheckbox = uiSimpleStuff::createCheckbox(mWidgetsManager, "SymmetricChanges", vec2(0, 0),
		"Symmetric changes", mSymmetricChanges);
	symmetricChangesCheckbox->bindValue(&mSymmetricChanges)
		->setChangeValueCallback(new cCallback<VehicleCreatorWidnow>(this, &VehicleCreatorWidnow::onSymmetricChangesChkBoxChanged));

	verLayoutWidget->addChild(symmetricChangesCheckbox);

//chassis
	mLeftForwardChassis.create(verLayoutWidget, 
		static_cast<phVehicleChassisComponent*>(mVehicle->getComponent("forwardLeftChassis")), "Forward Left Chassis");
	
	mRightForwardChassis.create(verLayoutWidget, 
		static_cast<phVehicleChassisComponent*>(mVehicle->getComponent("forwardRightChassis")), "Forward Right Chassis");
	
	mLeftRearChassis.create(verLayoutWidget, 
		static_cast<phVehicleChassisComponent*>(mVehicle->getComponent("rearLeftChassis")), "Rear Left Chassis");
	
	mRightRearChassis.create(verLayoutWidget, 
		static_cast<phVehicleChassisComponent*>(mVehicle->getComponent("rearRightChassis")), "Rear Right Chassis");

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
	mVehicle->mPosition = vec3(0, 0, 0);
	mVehicle->setOrient(nullMatr());
	mVehicle->mVelocity = vec3(0, 0, 0);
}

void VehicleCreatorWidnow::onSymmetricChangesChkBoxChanged()
{

}

void VehicleCreatorWidnow::ChassisEditContainer::create( uiWidget* parentWidget, phVehicleChassisComponent* chassis,
	const std::string& name )
{
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
	mPosX->bindValue(&chassis->mLocalPosition.x);

	mPosY = uiSimpleStuff::createTextEdit(widgetsManager, vec2(0, 0), vec2(50, 22), "yEdit");
	mPosY->bindValue(&chassis->mLocalPosition.y);

	mPosZ = uiSimpleStuff::createTextEdit(widgetsManager, vec2(0, 0), vec2(50, 22), "zEdit");
	mPosZ->bindValue(&chassis->mLocalPosition.z);
	
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
		mAngleX, parentWidget);

	addPropertyWithScrollbar(widgetsManager, "AngleY", "Y", mAngleYEdit, 
		new cCallback<ChassisEditContainer>(this, &ChassisEditContainer::anglesChanged), -30.0f, 30.0f, mAngleYScrollbar, 
		mAngleY, parentWidget);

	addPropertyWithScrollbar(widgetsManager, "AngleZ", "Z", mAngleZEdit, 
		new cCallback<ChassisEditContainer>(this, &ChassisEditContainer::anglesChanged), -30.0f, 30.0f, mAngleZScrollbar, 
		mAngleZ, parentWidget);
	
//angles
	uiLabel* wposCaption = uiSimpleStuff::createLabel(widgetsManager, vec2(0, 0), vec2(100, 20), name + "WPosCaption", "Wheel Pos");
	wposCaption->setHorAlign(uiLabel::AL_LEFT);

	parentWidget->addChild(wposCaption);
	
	addPropertyWithScrollbar(widgetsManager, "MinPos", "min", mMinPos, NULL, -1.0f, 1.0f, mMinPosScrollbar, 
		chassis->mMinPosition, parentWidget);
	
	addPropertyWithScrollbar(widgetsManager, "MaxPos", "max", mMaxPos, NULL, -1.0f, 1.0f, mMaxPosScrollbar, 
		chassis->mMaxPosition, parentWidget);
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
	scrollbar->bindValue(&bindingValue);

	propHorLayout->addChild(captionLabel);
	propHorLayout->addChild(textEdit);
	propHorLayout->addChild(scrollbar);

	parentWidget->addChild(propHorLayout);

	return bindValue;
}
