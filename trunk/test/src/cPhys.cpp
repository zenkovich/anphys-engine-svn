#include "cPhys.h"

#include <fstream>

cCar::cCar(vec3& Position, quat& Rotation)
{
	Vehicle = new physics::Vehicle();

	mAccumDT = 0;
	mMaxDeltaTime = 0.01f;

	vec3 size(1.8f, 1.1f, 3.7f); //ориентировочный размер авто, вполне может не совподать с реальными размерами

	Vehicle->mPosition = physics::vec3(Position.x, Position.z, Position.y);        //позиция
	//Vehicle->mPosition = physics::vec3(40, 2, 40);        //позиция
	Vehicle->mOrient = physics::mat3x3();               //ориентация
	Vehicle->mVelocity = physics::vec3(0, 0, 0);        //скорость
	Vehicle->mAngularVelocity = physics::vec3(0, 0, 0); //угловая скорость
	Vehicle->mMass = 1500;                              //масса
	Vehicle->mInvMass = 1.0f/Vehicle->mMass;           

	//расчет момента инерции
	float koef=2.0f*Vehicle->mMass*0.05f;
	Vehicle->mInertia = physics::mat3x3((size.y*size.y+size.z*size.z)*koef,0,0,
				                        0,(size.x*size.x+size.z*size.z)*koef,0,
				                        0,0,(size.y*size.y+size.x*size.x)*koef);
				  
	Vehicle->mInvInertia = Vehicle->mInertia.inverse();
	Vehicle->mWorldInertia = Vehicle->mInertia;
	Vehicle->mInvWorldInertia = Vehicle->mInvInertia;

	//параметры шасси
	float peakValue = 252000;                                 //нагрузка на шину, при которой начинается скольжение
	float frictionValues[] = {1.0f, 1.0f, 0.9f, 0.8f, 0.8f};  //примерный график трения шины. Можно использовать такой везде, лишь менять на глаз peakValue

	//заполнение базовых параметров для каждого шасси   
	Vehicle->mFrontLeftChassis->loadParametres(physics::vec3(-0.85f, -0.2f, 1.35f), //позиция относительно центра масс/
										       physics::mat3x3(),                  //ориентация оси (лучше не менять)
										       0,                                  //минимальная позиция амортизатора
										       -0.3f,                              //максимальная позиция амотризатора. При данных параметрах ход подвески составляет 30 см
										       0.33f,                              //радиус колеса
										       20.0f,                              //маccа колеса (кг)
										       80000.0f,                           //упругость пружины
										       3000.0f,                            //сила амортизатора
										       3000.0f,                            //усилие при торможении
										       0.0f);                              //усилие при торможении ручником
	Vehicle->mFrontRightChassis->loadParametres(physics::vec3(0.85f, -0.2f, 1.35f), physics::mat3x3(), 0, -0.3f, 0.33f, 20.0f, 80000.0f, 3000.0f, 3000.0f);
	Vehicle->mRearLeftChassis->loadParametres(physics::vec3(-0.85f, -0.2f, -1.27f), physics::mat3x3(), 0, -0.3f, 0.33f, 20.0f, 80000.0f, 3000.0f, 3000.0f, 100000000.0f);
	Vehicle->mRearRightChassis->loadParametres(physics::vec3(0.85f, -0.2f, -1.27f), physics::mat3x3(), 0, -0.3f, 0.33f, 20.0f, 80000.0f, 3000.0f, 3000.0f, 100000000.0f); 

	Vehicle->mFrontLeftChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	Vehicle->mFrontRightChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	Vehicle->mRearLeftChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	Vehicle->mRearRightChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);

	//параметры двигателя/трансмиссии
	const int torqueValuesCount = 8;  //количество значений графика
	float torqueGraphic[torqueValuesCount] = { 0.2, 0.64f, 0.84f, 0.99f, 0.96f, 1.0f, 0.92f, 0.2f }; //приблизительный единичный график крутящего момента
	float maxTorque = 600.0f;     //макс крутящий момент. !! Впринципе достаточно менять его и макс. кол-во оборотов
	float maxRpm = 7300.0f;       //макс кол-во оборотов
	float engineFriction = 0.03f; //внутреннее трение двигателя
	for (int i = 0; i < torqueValuesCount; i++)
	{
		torqueGraphic[i] = torqueGraphic[i]*maxTorque;
	}

	Vehicle->setEngineParams(torqueGraphic, torqueValuesCount, maxRpm, 1000.0f, 0.3f, engineFriction);

	const int gearsCount = 7; //количество передач вместе с задней и нейтральной. 
	float gears[gearsCount] = { -4.0f, 0.0f, 3.64f, 1.95f, 1.36f, 0.94f, 0.78f }; //передаточные отношения передач. 
	//Первое число - задняя передача, второе - нейтральная, третье - первая передача и т.д. 
	float topgear = 3.9f; //главная пара

	Vehicle->setGearBoxParametres(gears, gearsCount, topgear, physics::Vehicle::WD_REAR);
	Vehicle->setupAeroCoefs(0.001f, physics::vec3(0, -0.4f, -2), 0.01f, physics::vec3(0, 0, 0), 0.01f, 
		physics::vec3(0, -0.1f, 2), 0.01f);

	//точки геометрии, сталкивающейся с ландшафтом
	//в данном случае заполняются 8 точек для параллелепипеда
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
	frictionCoefs[0] = 1.0f;
	frictionCoefs[3] = 0.8f;

	Vehicle->setupLanscapeFrtCoefs(frictionCoefs, 4);
	
	float depthCoefs[256];
	memset(depthCoefs, 0, sizeof(float)*256);
	depthCoefs[0] = 0.0f;
	depthCoefs[3] = 0.0f;

	Vehicle->setupLanscapeDepthCoefs(depthCoefs, 4);
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
		clearDbgLines();
		Vehicle->update(dt);
	}
	else
	{	
		mAccumDT += dt;

		while (mAccumDT > mMaxDeltaTime)
		{
			mAccumDT -= mMaxDeltaTime;
			clearDbgLines();
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

unsigned char cCar::getWheelFrictionCoefIdx( int idx )
{
	if (idx == 0) 
		return Vehicle->mFrontLeftChassis->getFrictionIdx();
	if (idx == 1) 
		return Vehicle->mFrontRightChassis->getFrictionIdx();
	if (idx == 2) 
		return Vehicle->mRearLeftChassis->getFrictionIdx();

	return Vehicle->mRearRightChassis->getFrictionIdx();
}

float cCar::getWheelSlideCoef( int idx )
{	
	if (idx == 0) 
		return Vehicle->mFrontLeftChassis->mSlideCoef;
	if (idx == 1) 
		return Vehicle->mFrontRightChassis->mSlideCoef;
	if (idx == 2) 
		return Vehicle->mRearLeftChassis->mSlideCoef;

	return Vehicle->mRearRightChassis->mSlideCoef;
}

float cCar::getWheelRPM( int idx )
{
	if (idx == 0) 
		return Vehicle->mFrontLeftChassis->mWheelAngVelocity*60.0f;
	if (idx == 1) 
		return Vehicle->mFrontRightChassis->mWheelAngVelocity*60.0f;
	if (idx == 2) 
		return Vehicle->mRearLeftChassis->mWheelAngVelocity*60.0f;

	return Vehicle->mRearRightChassis->mWheelAngVelocity*60.0f;
}

int cCar::getDbgLinesCount()
{
	return Vehicle->mDbgLines.size();
}

void cCar::getDbgLine( int idx, float* p1, float* p2, float* colr )
{
	physics::vmask(p1, Vehicle->mDbgLines[idx].p1);
	physics::vmask(p2, Vehicle->mDbgLines[idx].p2);
	colr[0] = Vehicle->mDbgLines[idx].cr;
	colr[1] = Vehicle->mDbgLines[idx].cg;
	colr[2] = Vehicle->mDbgLines[idx].cb;
	colr[3] = Vehicle->mDbgLines[idx].ca;
}

void cCar::clearDbgLines()
{
	Vehicle->mDbgLines.clear();
}

void cCar::reset( vec3& pos )
{
	Vehicle->mPosition = physics::vmask((float*)&pos);
	Vehicle->mOrient = physics::mat3x3();
}

void cCar::moveStp( float x, float y, float z )
{
	Vehicle->mCollisionGeometryPoints[0].mLocalPos += physics::vec3(x, y, z)*0.5f;
	printf("stp %.3f %.3f %.3f\n", Vehicle->mCollisionGeometryPoints[0].mLocalPos.x, Vehicle->mCollisionGeometryPoints[0].mLocalPos.y, Vehicle->mCollisionGeometryPoints[0].mLocalPos.z);
}

void cCar::pushPoint()
{
	Vehicle->mCollisionGeometryPoints.push_back(Vehicle->mCollisionGeometryPoints[0]);

	std::ofstream fout;
	fout.open("cp.txt");

	for (physics::Vehicle::PointsList::iterator it = Vehicle->mCollisionGeometryPoints.begin();
		it != Vehicle->mCollisionGeometryPoints.end(); ++it)
	{
		//physics::vec3(halsSize.x, -halsSize.y, -halsSize.z)
		fout << "physics::vec3( " << it->mLocalPos.x << ", " << it->mLocalPos.y << ", " << it->mLocalPos.z << "), \n";
	}

	fout.close();
}
