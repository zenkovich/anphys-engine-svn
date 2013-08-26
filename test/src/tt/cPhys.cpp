#include "cPhys.h"

#include <fstream>

cCar::cCar(vec3& Position, quat& Rotation)
{
	Vehicle = new physics::Vehicle();

	mAccumDT = 0;
	mMaxDeltaTime = 0.01f;

	vec3 size(1.8f, 1.1f, 3.7f); //��������������� ������ ����, ������ ����� �� ��������� � ��������� ���������

	Vehicle->mPosition = physics::vec3(Position.x, Position.z, Position.y);        //�������
	//Vehicle->mPosition = physics::vec3(40, 2, 40);        //�������
	Vehicle->mOrient = physics::mat3x3();               //����������
	Vehicle->mVelocity = physics::vec3(0, 0, 0);        //��������
	Vehicle->mAngularVelocity = physics::vec3(0, 0, 0); //������� ��������
	Vehicle->mMass = 1500;                              //�����
	Vehicle->mInvMass = 1.0f/Vehicle->mMass;           

	//������ ������� �������
	float koef=2.0f*Vehicle->mMass*0.05f;
	Vehicle->mInertia = physics::mat3x3((size.y*size.y+size.z*size.z)*koef,0,0,
				                        0,(size.x*size.x+size.z*size.z)*koef,0,
				                        0,0,(size.y*size.y+size.x*size.x)*koef);
				  
	Vehicle->mInvInertia = Vehicle->mInertia.inverse();
	Vehicle->mWorldInertia = Vehicle->mInertia;
	Vehicle->mInvWorldInertia = Vehicle->mInvInertia;

	//��������� �����
	float peakValue = 252000;                                 //�������� �� ����, ��� ������� ���������� ����������
	float frictionValues[] = {1.0f, 1.0f, 0.9f, 0.8f, 0.8f};  //��������� ������ ������ ����. ����� ������������ ����� �����, ���� ������ �� ���� peakValue

	//���������� ������� ���������� ��� ������� �����   
	Vehicle->mFrontLeftChassis->loadParametres(physics::vec3(-0.85f, -0.2f, 1.35f), //������� ������������ ������ ����/
										       physics::mat3x3(),                  //���������� ��� (����� �� ������)
										       0,                                  //����������� ������� ������������
										       -0.3f,                              //������������ ������� ������������. ��� ������ ���������� ��� �������� ���������� 30 ��
										       0.33f,                              //������ ������
										       70.0f,                              //��cc� ������ (��)
										       80000.0f,                           //��������� �������
										       3000.0f,                            //���� ������������
										       3000.0f,                            //������ ��� ����������
										       0.0f);                              //������ ��� ���������� ��������
	Vehicle->mFrontRightChassis->loadParametres(physics::vec3(0.85f, -0.2f, 1.35f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 3000.0f);
	Vehicle->mRearLeftChassis->loadParametres(physics::vec3(-0.85f, -0.2f, -1.27f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 3000.0f, 100000000.0f);
	Vehicle->mRearRightChassis->loadParametres(physics::vec3(0.85f, -0.2f, -1.27f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 3000.0f, 100000000.0f); 

	Vehicle->mFrontLeftChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	Vehicle->mFrontRightChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	Vehicle->mRearLeftChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	Vehicle->mRearRightChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);

	//��������� ���������/�����������
	const int torqueValuesCount = 8;  //���������� �������� �������
	float torqueGraphic[torqueValuesCount] = { 0, 0.4f, 0.64f, 0.8f, 0.96f, 1.0f, 0.92f, 0.2f }; //��������������� ��������� ������ ��������� �������
	float maxTorque = 800.0f;     //���� �������� ������. !! ��������� ���������� ������ ��� � ����. ���-�� ��������
	float maxRpm = 7000.0f;       //���� ���-�� ��������
	float engineFriction = 0.04f; //���������� ������ ���������
	for (int i = 0; i < torqueValuesCount; i++)
	{
	torqueGraphic[i] = torqueGraphic[i]*maxTorque + maxRpm/(float)(torqueValuesCount - 1)*(float)(i)*engineFriction;
	}

	Vehicle->setEngineParams(torqueGraphic, torqueValuesCount, maxRpm, 1000.0f, 0.6f, engineFriction);

	const int gearsCount = 7; //���������� ������� ������ � ������ � �����������. 
	float gears[gearsCount] = { -4.0f, 0.0f, 3.64f, 1.95f, 1.36f, 0.94f, 0.78f }; //������������ ��������� �������. 
	//������ ����� - ������ ��������, ������ - �����������, ������ - ������ �������� � �.�. 
	float topgear = 3.9f; //������� ����

	Vehicle->setGearBoxParametres(gears, gearsCount, topgear, physics::Vehicle::WD_REAR);

	//����� ���������, �������������� � ����������
	//� ������ ������ ����������� 8 ����� ��� ���������������
	physics::vec3 halsSize(size.x*0.5f, size.y*0.5f, size.z*0.5f);
	physics::vec3 collisionPoints[] = { physics::vec3(-halsSize.x, -halsSize.y, -halsSize.z), 
								physics::vec3(halsSize.x, -halsSize.y, -halsSize.z), 
								physics::vec3(halsSize.x, halsSize.y, -halsSize.z), 
								physics::vec3(-halsSize.x, halsSize.y, -halsSize.z), 
									
								physics::vec3(-halsSize.x, -halsSize.y, halsSize.z), 
								physics::vec3(halsSize.x, -halsSize.y, halsSize.z), 
								physics::vec3(halsSize.x, halsSize.y, halsSize.z), 
								physics::vec3(-halsSize.x, halsSize.y, halsSize.z) };
	int pointsCount = 8;
	for (int i = 0; i < pointsCount; i++)
	{
		Vehicle->mCollisionGeometryPoints.push_back(physics::CollisionGeometryVertex(collisionPoints[i]));
	}

	float frictionCoefs[256];
	memset(frictionCoefs, 0, sizeof(float)*256);
	frictionCoefs[0] = 1;
	frictionCoefs[3] = 0.6f;

	Vehicle->setupLanscapeFrtCoefs(frictionCoefs, 4);
}

vec3 cCar::getPosition(){
	vec3 p;
	Vehicle->getPosition(&p.x);
	/*float t = p.y;
	p.y = p.z;
	p.z = t;*/
	return p;
}

void cCar::update(float dt, cEngineLandscapeVertex** vBuffer, cEngineLandscapePoly** pBuffer, int PolyCount)
{
	Vehicle->mClutchCoef = 1.0f;
	Vehicle->setPolygonsBuffer((physics::lPolygon**)pBuffer, (physics::lVertex**)vBuffer, PolyCount);

	if (dt < mMaxDeltaTime)
	{
		Vehicle->update(dt);
	}
	else
	{	
		mAccumDT += dt;

		while (mAccumDT > mMaxDeltaTime)
		{
			mAccumDT -= mMaxDeltaTime;
			Vehicle->update(mMaxDeltaTime);
		}
	}
}

void cCar::getRotation( float* arr )
{
	float matr[9];
	Vehicle->getOrientation(matr);
	physics::matrix2quat(matr, arr);
}

void cCar::setSteerWheelAngle( float angle )
{
	Vehicle->mSteerWheelAngle = angle;
}

void cCar::setThrottleCoef( float coef )
{
	Vehicle->mThrottleCoef = coef;
}

void cCar::setBrakesCoef( float coef )
{
	Vehicle->mBrakeCoef = coef;
}

void cCar::setHandBrakeCoef( float coef )
{
	Vehicle->mHandBrakeCoef = coef;
}

void cCar::gearUp()
{
	Vehicle->mCurrentGear++;
}

void cCar::gearDown()
{
	Vehicle->mCurrentGear--;
}

vec3 cCar::getWheelPos( int i )
{
	vec3 res;
	if (i == 0) 
		Vehicle->mFrontLeftChassis->getPosition((float*)&res);
	if (i == 1) 
		Vehicle->mFrontRightChassis->getPosition((float*)&res);
	if (i == 2) 
		Vehicle->mRearLeftChassis->getPosition((float*)&res);
	if (i == 3) 
		Vehicle->mRearRightChassis->getPosition((float*)&res);

	return res;
}

void cCar::getWheelOrient( float* matr, int idx )
{
	if (idx == 0) 
		Vehicle->mFrontLeftChassis->getOrientation(matr);
	if (idx == 1) 
		Vehicle->mFrontRightChassis->getOrientation(matr);
	if (idx == 2) 
		Vehicle->mRearLeftChassis->getOrientation(matr);
	if (idx == 3) 
		Vehicle->mRearRightChassis->getOrientation(matr);
}

void cCar::reset( vec3& pos )
{
	Vehicle->mPosition = physics::vmask((float*)&pos);
	Vehicle->mOrient = physics::mat3x3();
}