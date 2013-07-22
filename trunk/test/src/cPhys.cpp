#include "cPhys.h"



cCar::cCar(vec3& Position, quat& Rotation)
{
	Vehicle = new physics::Vehicle();

	vec3 size(1.8f, 1.1f, 3.7f); //��������������� ������ ����, ������ ����� �� ��������� � ��������� ���������

	Vehicle->mPosition = physics::vec3(Position.x, Position.y, Position.z);        //�������
	Vehicle->mOrient = physics::mat3x3();               //����������
	Vehicle->mVelocity = physics::vec3(0, 0, 2);        //��������
	Vehicle->mAngularVelocity = physics::vec3(0, 0, 0); //������� ��������
	Vehicle->mMass = 1500;                              //�����
	Vehicle->mInvMass = 1.0f/Vehicle->mMass;           

	//������ ������� �������
	float koef=2.0f*Vehicle->mMass*0.1f;
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
	Vehicle->mFrontLeftChassis->loadParametres(physics::vec3(-0.95f, -0.2f, 1.5f), //������� ������������ ������ ����/
										physics::mat3x3(),                  //���������� ��� (����� �� ������)
										0,                                  //����������� ������� ������������
										-0.3f,                              //������������ ������� ������������. ��� ������ ���������� ��� �������� ���������� 30 ��
										0.33f,                              //������ ������
										70.0f,                              //��cc� ������ (��)
										80000.0f,                           //��������� �������
										3000.0f,                            //���� ������������
										1000.0f,                            //������ ��� ����������
										0.0f);                              //������ ��� ���������� ��������
	Vehicle->mFrontRightChassis->loadParametres(physics::vec3(0.95f, -0.2f, 1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f);
	Vehicle->mRearLeftChassis->loadParametres(physics::vec3(-0.95f, -0.2f, -1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f, 100000000.0f);
	Vehicle->mRearRightChassis->loadParametres(physics::vec3(0.95f, -0.2f, -1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f, 100000000.0f); 

	Vehicle->mFrontLeftChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	Vehicle->mFrontRightChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	Vehicle->mRearLeftChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	Vehicle->mRearRightChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);

	//��������� ���������/�����������
	const int torqueValuesCount = 8;  //���������� �������� �������
	float torqueGraphic[torqueValuesCount] = { 0, 0.4f, 0.64f, 0.8f, 0.96f, 1.0f, 0.92f, 0.2f }; //��������������� ��������� ������ ��������� �������
	float maxTorque = 500.0f;     //���� �������� ������. !! ��������� ���������� ������ ��� � ����. ���-�� ��������
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
	Vehicle->setPolygonsBuffer((physics::lPolygon**)pBuffer, (physics::lVertex**)vBuffer, PolyCount);
	Vehicle->update(dt);
}

void cCar::getRotation( float* arr )
{
	float matr[9];
	Vehicle->getOrientation(matr);
	physics::matrix2quat(matr, arr);
}
