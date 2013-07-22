#include "game.h"
#include "cPhys.h"
#include <windows.h>
#include <math.h>
#include <vector>



PELOADLEVELPROC eLoadLevel = 0;
PEGETCAMERAPROC eGetCamera = 0;
PESETCAMERAPROC eSetCamera = 0;
PEGETMODELPROC  eGetModel = 0;
PESETMODELSCOUNTPROC eSetModelsCount = 0;
PESETMODELTPROC eSetModel = 0;
PEGETHEIGHTPROC eGetHeight = 0;
PEGETFONTPROC eGetFont = 0;
PETEXTOUTPROC eTextOut = 0;
PEGETOBJECTATTRIBUTESPROC eGetObjectAttributes = 0;
PEGETINTERACTIVEOBJECTPROC eGetInteractiveObject = 0;
PEGETMOUSE3DPROC eGetMouse3D = 0;
PEGETLANDSCAPEMESHPROC eGetLandscapeMesh = 0;
PEGETLANDSCAPEZONEMESHPROC eGetLandscapeZoneMesh = 0;
PESETOBJECTPOSITIONPROC eSetObjectPosition = 0;
PESETOBJECTROTATIONPROC eSetObjectRotation = 0;
PEDEBUGDRAWLINEPROC eDebugDrawLine = 0;
PEGETLANDSCAPEINFOPROC eGetLandscapeInfo = 0;

vec3 PlayerPosition;
vec3 PlayerRotation(0,-80,0);
bool MoveForward = false;
bool MoveBackward = false;
bool FreeView = false;
bool DrawBulletDebug = false;
float MousePos[2];

cEngineFont FontImpact12;

//Функция вызывается один раз, при старте движка
__declspec(dllexport) cEngine __cdecl engineInit(char* Path, PENGINEGETPROCADDRESSPROC engineGetProcAddress, int Width, int Height)
{
	//получаем указатели на нужные нам методы
	eLoadLevel      = (PELOADLEVELPROC)engineGetProcAddress("eLoadLevel");
	eGetCamera      = (PEGETCAMERAPROC)engineGetProcAddress("eGetCamera");
	eSetCamera      = (PESETCAMERAPROC)engineGetProcAddress("eSetCamera");
	eGetModel       = (PEGETMODELPROC)engineGetProcAddress("eGetModel");
    eSetModelsCount = (PESETMODELSCOUNTPROC)engineGetProcAddress("eSetModelsCount");
    eSetModel       = (PESETMODELTPROC)engineGetProcAddress("eSetModel");
	eGetHeight      = (PEGETHEIGHTPROC)engineGetProcAddress("eGetHeight");
	eGetFont        = (PEGETFONTPROC)engineGetProcAddress("eGetFont");
	eTextOut        = (PETEXTOUTPROC)engineGetProcAddress("eTextOut");
	eGetObjectAttributes = (PEGETOBJECTATTRIBUTESPROC)engineGetProcAddress("eGetObjectAttributes");
	eGetInteractiveObject= (PEGETINTERACTIVEOBJECTPROC)engineGetProcAddress("eGetInteractiveObject");
	eGetMouse3D     = (PEGETMOUSE3DPROC)engineGetProcAddress("eGetMouse3D");
	eGetLandscapeMesh = (PEGETLANDSCAPEMESHPROC)engineGetProcAddress("eGetLandscapeMesh");
	eGetLandscapeZoneMesh = (PEGETLANDSCAPEZONEMESHPROC)engineGetProcAddress("eGetLandscapeZoneMesh");
	eSetObjectPosition = (PESETOBJECTPOSITIONPROC)engineGetProcAddress("eSetObjectPosition");
    eSetObjectRotation = (PESETOBJECTROTATIONPROC)engineGetProcAddress("eSetObjectRotation");
	eDebugDrawLine = (PEDEBUGDRAWLINEPROC)engineGetProcAddress("eDebugDrawLine");
	eGetLandscapeInfo = (PEGETLANDSCAPEINFOPROC)engineGetProcAddress("eGetLandscapeInfo");

	eLoadLevel("test.gdl");  //Грузим уровень
	//eLoadLevel("race1_v_2.gdl");  //Грузим уровень
	return 0;
}

__declspec(dllexport) void __cdecl engineRelease(cEngine Engine)
{
}
struct cCarAttributes{
	cCar* Car;
};

cEngineInteractiveObject Car = 0;

cEngineLandscapeVertex** LandVerticies;
cEngineLandscapePoly** LandIndexes;
int LandPolyCount = 0;

__declspec(dllexport) void __cdecl engineProcessEvent(cEngine Engine, cEvent* Info)
{
	switch(Info->Type){
	case E_DRAWINTERFACE:{
						 }
	case E_UPDATE:{
						cEventUpdate* Event = (cEventUpdate*)Info;
						
						float dt = Event->dt/1000.0f;

						
						if (Car!=0){
							cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
							#ifdef ANPHYS_ON
							vec3 p = attribs->Car->getPosition();
							#else
							vec3 p,r,v;
							eGetMouse3D(&p,&r,&v);
							#endif
							eGetLandscapeZoneMesh(p.x-2.5,p.y-2.5, 5.0,5.0, &LandVerticies, &LandIndexes, &LandPolyCount);
							attribs->Car->update(dt, LandVerticies, LandIndexes, LandPolyCount); //передаем массивы полигонов

							vec3 carPos = attribs->Car->getPosition();
							/*
							cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
							attribs->Car->Update(dt);*/
							//btTransform tr =attribs->Car->getBody()->getWorldTransform();
							//btQuaternion r = tr.getRotation();
							//quat Rot = quat(r.x(),r.y(),r.z(),-r.w());
							//eSetObjectRotation(Car, &Rot);
							//btVector3 p = tr.getOrigin();
							//vec3 Pos = vec3(p.x(),p.y(),p.z()+0.2);;
							eSetObjectPosition(Car, &carPos);

							float orient[4];
							attribs->Car->getRotation(orient);
							quat Rot = quat(orient[0],orient[2],orient[1],orient[3]);
							eSetObjectRotation(Car, &Rot);
							
							//PlayerPosition.z = carPos.z;
							vec3 Dir = carPos-PlayerPosition;
							Dir.normalize();
							//PlayerRotation.x = AngleBetweenVectors(0,1, Dir.x,Dir.y)/3.14*180.0;

							Dir = Dir * 4.0;
							/*PlayerPosition = carPos - Dir;
							PlayerPosition.z += 3.0;*/
						};
						
						if (MoveForward){
							PlayerPosition.x += sin(PlayerRotation.x*3.14f/180.0f)*dt*10;
							PlayerPosition.y += cos(PlayerRotation.x*3.14f/180.0f)*dt*10;
						};
						if (MoveBackward){
							PlayerPosition.x -= sin(PlayerRotation.x*3.14f/180.0f)*dt*10;
							PlayerPosition.y -= cos(PlayerRotation.x*3.14f/180.0f)*dt*10;
						};
						
						float height = eGetHeight(PlayerPosition.x,PlayerPosition.y,true)+1.0f;	///+1.0, потому что камера должна все таки на некоторой высоте ездить над ландшафтом					
						if (PlayerPosition.z>height)
							PlayerPosition.z -= 2.0f*dt; //У нас типа антиграв же, поэтому падаем медленно и равномерно
						if (PlayerPosition.z<height)
							PlayerPosition.z = height;
						eSetCamera(-1, &PlayerPosition, &PlayerRotation);					
				  }		
		break;
	case E_MOUSEMOVE:{
						cEventMouseMove* Event = (cEventMouseMove*)Info;
						if (FreeView){
							PlayerRotation.x += Event->x-MousePos[0];
							PlayerRotation.y += Event->y-MousePos[1];
							if (PlayerRotation.y>-45)
								PlayerRotation.y=-45;
							if (PlayerRotation.y<-135)
								PlayerRotation.y=-135;
						}
						MousePos[0] = Event->x;
						MousePos[1] = Event->y;
					 }
		break;
	case E_KEYUP:{
						cEventKey* Event = (cEventKey*)Info;
						switch(Event->Key){
							case VK_UP:{
											//cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
											//attribs->Car->Accelerate(false);
									   }
								break;
							case VK_DOWN:{
											//cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
											//attribs->Car->Break(false);
									   }
								break;
							case VK_LEFT:{
											//cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
											//attribs->Car->Rotate(0);
									   }
								break;
							case VK_RIGHT:{
											//cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
											//attribs->Car->Rotate(0);
									   }
								break;
							case 'W':MoveForward = false;
								break;
							case 'S':MoveBackward = false;
								break;
							case VK_F12:DrawBulletDebug = false;
								break;
							case VK_LBUTTON:
								FreeView = false;
								
						};
				 }
		break;
	case E_KEYDOWN:{
						cEventKey* Event = (cEventKey*)Info;
						switch(Event->Key){
							case VK_UP:{
											//cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
											//attribs->Car->Accelerate(true);
									   }
								break;
							case VK_DOWN:{
											//cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
											//attribs->Car->Break(true);
									   }
								break;
							case VK_LEFT:{
											//cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
											//attribs->Car->Rotate(+1);
									   }
								break;
							case VK_RIGHT:{
											//cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
											//attribs->Car->Rotate(-1);
									   }
								break;

							case 'W':MoveForward = true;
								break;
							case 'S':MoveBackward = true;
								break;
							case VK_F12:DrawBulletDebug = true;
								break;
							case VK_LBUTTON:
								FreeView = true;
								break;
						};
				   }
		break;
	case E_LEVELINIT:{
						//
		/*
						vec3* VertexList;
						int* IndexList;
						int VertexCount,IndexCount;
						cEngineLandscapeInfo* Info = eGetLandscapeInfo();
						for (int y = 0; y<Info->Height; y++)
							for (int x = 0; x<Info->Width; x++)
								if (eGetLandscapeMesh(x,y, &VertexList, &IndexList, &VertexCount, &IndexCount)){
									//Phys->AddLandscape(VertexList, IndexList, VertexCount, IndexCount);
									printf("x:%i y:%i",x,y);
								}*/
					 }
	};
}

enum IObjectUIDS{
	IO_PLAYER = 0
};

cEngineModel CarModel = 0;
__declspec(dllexport) void __cdecl initBaseInteractiveObject(cEngine Engine, IObjectUIDS InteractiveObjectUID, void* Attributes)
{
	switch(InteractiveObjectUID){
		case IO_PLAYER:
			if (CarModel==0)
				CarModel = eGetModel("cars//","audiS4.gdt");
			break;
	}
}

__declspec(dllexport) void __cdecl releaseBaseInteractiveObject(cEngine Engine, IObjectUIDS InteractiveObjectUID, void* Attributes)
{
	//нечего нам тут делать
}


void SetColor(float* C, float r,float g, float b, float a)
{
	C[0] = r;
	C[1] = g;
	C[2] = b;
	C[3] = a;
}

void SetIdenityMatrix(float* M)
{
	for (int i = 0; i<16; i++)
		M[i] = 0;
	M[0] = 1;
	M[5] = 1;
	M[10]= 1;
	M[15]= 1;
}

__declspec(dllexport) void __cdecl duplicateInteractiveObject(cEngine Engine, IObjectUIDS InteractiveObjectUID, void* Attributes, void* BaseAttributes, cEngineInteractiveObject Object, vec3 *Position, quat *Rotation)
{
	switch(InteractiveObjectUID){
		case IO_PLAYER:{
							cIOModel Model;
							Model.Animation = 0;
							SetColor(Model.Color,1,1,1,1);
							Model.Model = CarModel;
							SetIdenityMatrix(Model.Transform);
							Model.Shader = 0;
							Model.Uniforms = 0;
							Model.ZWrite = true;

							cCarAttributes* Attrib = (cCarAttributes*)Attributes;
							vec3 p = *Position;
							p.z = p.z+0.5f;
							PlayerPosition = p;
							Attrib->Car = new cCar(p, *Rotation);
							Car = Object;

							eSetModelsCount(Object, 1);
							eSetModel(Object, 0, Model);
					   }
			break;
	}
}

__declspec(dllexport) void __cdecl releaseInteractiveObject(cEngine Engine, IObjectUIDS InteractiveObjectUID, void* Attributes)
{
	switch(InteractiveObjectUID){
		case IO_PLAYER:{
							cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
							delete attribs->Car;
							Car = NULL;
					   }
			break;
	}
}

__declspec(dllexport) void __cdecl processEventInteractiveObject(cEngine Engine, IObjectUIDS InteractiveObjectUID, void* Attributes, cEvent* Info)
{
	//никак событий предназначеных объектам у нас нет, так что обрабатывать здесь нечего
}

__declspec(dllexport) void __cdecl drawDebug()
{
	//if (DrawBulletDebug)
	//	Phys->DebugDraw();

	for (int i = 0; i<LandPolyCount; i++){
		vec3* v1 = (vec3*)LandVerticies[LandIndexes[i]->v1];
		vec3* v2 = (vec3*)LandVerticies[LandIndexes[i]->v2];
		vec3* v3 = (vec3*)LandVerticies[LandIndexes[i]->v3];
		
		eDebugDrawLine(v1,v2,0,1,0,1);
		eDebugDrawLine(v2,v3,0,1,0,1);
		eDebugDrawLine(v3,v1,0,1,0,1);
	};

	if (Car!=0){							
		cCarAttributes* attribs = (cCarAttributes*)eGetObjectAttributes(Car);
		vec3 p = attribs->Car->getPosition();

		vec3 v1,v2;
		v1 = p;
		v1.x = p.x-0.5;
		v2 = p;
		v2.x = p.x+0.5;
		eDebugDrawLine(&v1,&v2, 1,0,0,1);

		v1 = p;
		v1.y = p.y-0.5;
		v2 = p;
		v2.y = p.y+0.5;
		eDebugDrawLine(&v1,&v2, 1,0,0,1);

		v1 = p;
		v1.z = p.z-0.5;
		v2 = p;
		v2.z = p.z+0.5;
		eDebugDrawLine(&v1,&v2, 1,0,0,1);

	}
}