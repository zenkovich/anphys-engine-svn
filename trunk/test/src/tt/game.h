#pragma once

#include <math.h>

//�������, ������� ���������� �� ������
enum eEvents{
  E_LEVELINIT     = 1,
  E_LEVELRELEASE  = 2,
  E_UPDATE        = 3,
  E_DRAWINTERFACE = 4,
  E_MOUSEMOVE     = 5,
  E_KEYUP         = 6,
  E_KEYDOWN       = 7,
  E_MOUSEWHEEL    = 8,
  E_LOSTFOCUS     = 9,
  E_INITPERMANENTFUNCTION = 10,
  E_INITLEVELCODE = 11
};

//����� ��������� ���� �������
struct cEvent{
	int Size;
	eEvents Type;
};

//����� ������� E_UPDATE. dt - ����� � ������������� � �������� �������. pause - �������� �� ������ �����
struct cEventUpdate{
	cEvent Event;
	int dt;
    bool pause;
};
//����� ������� E_MOUSEMOVE. x,y - ������� ����������
struct cEventMouseMove{
	cEvent Event;
	int x,y;
};
//����� ������� E_KEYUP � E_KEYDOWN. Key - ����������� ��� �������
struct cEventKey{
	cEvent Event;
	int Key;
};

typedef void* cEngine; 
typedef void* cEngineInteractiveObject; 
typedef void* cEngineModel;
typedef void* cEngineShader;
typedef void* cEngineUniformsList;
typedef int   cEngineFont;

//���������, ������� ������������ ��� ������� ������ �������
struct cIOModel{
	float Transform[16];           //������� ������������� ������ � ��������� ������������ �������!
	cEngineModel Model;
    float Animation;
    float Color[4];                 
    bool ZWrite;                   //����� �� ������ ������ � ������ ������� 
    cEngineShader Shader;          //������, ���� �������� NULL, ����� ������������ �����������
    cEngineUniformsList Uniforms;  //�������� �������, ���� �������� NULL ��� ������������ ������
};

#define EPSILON 0.00001f

struct vec2i{
	int x,y;
};

struct mat3x3{
	float m[3][3];
};

struct vec3{
	float x,y,z;
	vec3(){x = 0; y= 0; z = 0;};
	vec3(float setX, float setY, float setZ){x = setX; y = setY, z = setZ;};
	const vec3 operator+(const vec3& rv) const {return vec3(x+rv.x,y+rv.y,z+rv.z);};
	const vec3 operator-(const vec3& rv) const {return vec3(x-rv.x,y-rv.y,z-rv.z);};
	const vec3 operator*(const float& rv) const {return vec3(x*rv,y*rv,z*rv);};
	void normalize(){
		float length = sqrt(x*x+y*y+z*z);
		if (length<EPSILON) return;
		x /= length;
		y /= length;
		z /= length;
	};
};

struct quat{
	float x,y,z,w;
	quat(){x = 0; y = 0; z = 0; w = 1;};
	quat(float setX, float setY, float setZ, float setW){x=setX; y=setY; z=setZ; w=setW;};
	void normalize(){
		float l = sqrt(x*x+y*y+z*z+w*w);
		if (l<0.00001)
			return;
		x /= l;
		y /= l;
		z /= l;
		w /= l;
	};
};

struct cEngineLandscapeInfo{
	int Width;       //���������� ������ �� X
	int Height;      //���������� ������ �� Y
 	float BlockSize; //������ ������ ����� � ������
};

struct cEngineLandscapeVertex{
	float x,y,z;
	unsigned char material; 
};

struct cEngineLandscapePoly{
	unsigned int v1,v2,v3;
	float nx,ny,nz;
};

typedef void* (__cdecl * PENGINEGETPROCADDRESSPROC) (char* Name); 

//������� ������ ���������� ����
//ID - ����� ������� � ENUM - INTERACTIVE_OBJECTS.
//SetAttributes - ��������� �� ��������� �� ������, ������� ��������� � �������� ����� ���������� �������. � ���� ��������� ��������� ��������� �� �������� ������ ������� ��� �������� �� �������
//FillMask - ��������� �� �����. ���� ����� �������� 1, �� ���� ���� �� SetAttributes ����������� �������, ���� 0 - ������������ �������� �� ���������
//Size - ������ ������ � SetAttributes � FillMask. ���� ���������� �� ����������� � ������� - ����� ��������. ������� ��� ��������, ����� �� ������ �������� �����������, ���� ������� ���������
//Result - ��������� �� ������ ������. ����� ����� �����������, �� ������ �� �� �������. ����� ����������.
typedef cEngineInteractiveObject (__cdecl * PECREATEINTERACTIVEOBJECT)(int ID, void** SetAttributes, void* FillMask, int Size, vec3* Position, quat* Rotation);
//���������� ������� �������� �������. ������ ��������� �� �����, � � ��������� �����
typedef void (__cdecl * PEMARKOBJECTTODELETE) (cEngineInteractiveObject Object); 
//��������� �������. �������� �� ���������, � � ������������ ������. ������� ������� ����� ������� � ����� ������. ����������� �������� ���������� � ������� ���������� �����.
typedef void (__cdecl * PELOADLEVELPROC) (const char* FileName); 
//��������� ������� ������. -1 - �������� ������. ��������� ������ ����� �������� � ���������
typedef void (__cdecl * PEGETCAMERAPROC) (int id, vec3* Position, vec3* Rotation);
//��������� ������� ������. -1 - ������� ������. ��������� ������ ����� ���������� � ��������� �������
typedef void (__cdecl * PESETCAMERAPROC) (int id, vec3* Position, vec3* Rotation);
//�������� ������. ������ � �������� ������. ����� ������������ ��� �������� ������, � � ��������� �� �������. ���� �������� ������������ �������� data/models
typedef cEngineModel (__cdecl * PEGETMODELPROC) (const char* Path, const char* Model);
//������������� ���������� ������ ��� ������ � �������. ��� ����� ����� ��������� �������� �� ���������� �����
typedef void (__cdecl *PESETMODELSCOUNTPROC) (cEngineInteractiveObject Object, int ModelsSlotsCount);
//������������� ������ � ����.
typedef void (__cdecl *PESETMODELTPROC) (cEngineInteractiveObject Object, int ModelSlot, cIOModel Model);
//�������� ������ ��������� � �����. ������� ������ �������
typedef float (__cdecl *PEGETHEIGHTPROC) (float x, float y, bool Accurate);
//�������� ������. ����� �������� ���������.
typedef cEngineFont (__cdecl *PEGETFONTPROC) (const char* Font);
//����� ������. ����� ����� ������ ������ � ������� E_DRAWINTERFACE
typedef void (__cdecl *PETEXTOUTPROC) (cEngineFont Font, int x, int y, float r, float g, float b, float a, const char* Text);
//��������� ���������� �������
typedef void*(__cdecl *PEGETOBJECTATTRIBUTESPROC) (cEngineInteractiveObject Object);
//��������. �������� ������ �� ����.
typedef cEngineInteractiveObject(__cdecl *PEGETINTERACTIVEOBJECTPROC)(vec3* Position, quat* Direction, bool approximately);
//��������� ���������� ���� � ����. �������� ������� ���� � ���
typedef void (__cdecl *PEGETMOUSE3DPROC)(vec3 *Position, vec3 *OnScreenPosition, vec3 *View);
//��������� ���� � ���������. ��� ��� ������
typedef cEngineLandscapeInfo* (__cdecl *PEGETLANDSCAPEINFOPROC)();
//��������� ��������� ���������. ��� ������
typedef bool (__cdecl *PEGETLANDSCAPEMESHPROC)(int x, int y, vec3** VertexList, int** IndexList, int* VertexCount, int* IndexCount);
//��������� ��������� ��������� ���������. ��� ������
typedef bool (__cdecl *PEGETLANDSCAPEZONEMESHPROC)(float x, float y, float width, float height, cEngineLandscapeVertex*** VertexList, cEngineLandscapePoly*** PolyList, int* PolyCount);
//��������� ������� ������� � ����
typedef void (__cdecl *PESETOBJECTPOSITIONPROC)(cEngineInteractiveObject Object, vec3* Position);
//��������� ���������� ������� � ���� ������������
typedef void (__cdecl *PESETOBJECTROTATIONPROC)(cEngineInteractiveObject Object, quat* Rotation);
//��������� ���������� ������� � ���� ��������
typedef void (__cdecl *PESETOBJECTROTATIONMATRIXPROC)(cEngineInteractiveObject Object, mat3x3* Rotation);
//��������� ���������� �����. �������� ������ � �������������� ������ __declspec(dllexport) void __cdecl drawDebug()
typedef void (__cdecl *PEDEBUGDRAWLINEPROC)(vec3* start, vec3* end, float r, float g, float b, float a);


extern PECREATEINTERACTIVEOBJECT eCreateInteractiveObject;
extern PEMARKOBJECTTODELETE eMarkObjectToDelete;
extern PELOADLEVELPROC eLoadLevel;
extern PEGETCAMERAPROC eGetCamera;
extern PESETCAMERAPROC eSetCamera;
extern PEGETMODELPROC  eGetModel;
extern PESETMODELSCOUNTPROC eSetModelsCount;
extern PESETMODELTPROC eSetModel;
extern PEGETHEIGHTPROC eGetHeight;
extern PEGETFONTPROC eGetFont;
extern PETEXTOUTPROC eTextOut;
extern PEGETOBJECTATTRIBUTESPROC eGetObjectAttributes;
extern PEGETINTERACTIVEOBJECTPROC eGetInteractiveObject;
extern PEGETMOUSE3DPROC eGetMouse3D;
extern PEGETLANDSCAPEINFOPROC eGetLandscapeInfo;
extern PEGETLANDSCAPEMESHPROC eGetLandscapeMesh;
extern PEGETLANDSCAPEZONEMESHPROC eGetLandscapeZoneMesh;
extern PESETOBJECTPOSITIONPROC eSetObjectPosition;
extern PESETOBJECTROTATIONPROC eSetObjectRotation;
extern PESETOBJECTROTATIONMATRIXPROC eSetObjectRotationMatrix;
extern PEDEBUGDRAWLINEPROC eDebugDrawLine;