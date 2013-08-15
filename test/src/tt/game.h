#pragma once

#include <math.h>

//События, которые передаются из движка
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

//Общий заголовок всех событий
struct cEvent{
	int Size;
	eEvents Type;
};

//Пакет события E_UPDATE. dt - время в миллисекундах с прошлого апдейта. pause - включена ли сейчас пауза
struct cEventUpdate{
	cEvent Event;
	int dt;
    bool pause;
};
//Пакет события E_MOUSEMOVE. x,y - оконные координаты
struct cEventMouseMove{
	cEvent Event;
	int x,y;
};
//Пакет события E_KEYUP и E_KEYDOWN. Key - виртуальный код клавиши
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

//Структура, которая используется для задания модели объекта
struct cIOModel{
	float Transform[16];           //Матрица трансформации модели в локальном пространстве объекта!
	cEngineModel Model;
    float Animation;
    float Color[4];                 
    bool ZWrite;                   //Нужно ли писать модель в буффер глубины 
    cEngineShader Shader;          //шейдер, надо передать NULL, чтобы использовать стандартный
    cEngineUniformsList Uniforms;  //юниформы шейдера, надо передать NULL для стандартного набора
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
	int Width;       //Количество блоков по X
	int Height;      //Количество блоков по Y
 	float BlockSize; //Размер одного блока в метрах
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

//Создает объект указанного типа
//ID - номер объекта в ENUM - INTERACTIVE_OBJECTS.
//SetAttributes - указатель на указатель на данные, которые запишутся в атрибуты вновь созданного объекта. В этот указатель запишется указатель на атрибуты нового объекта при возврате из функции
//FillMask - указатель на маску. Если маска содержит 1, то этот байт из SetAttributes установится объекту, если 0 - используется значение по умолчанию
//Size - размер данных в SetAttributes и FillMask. Если отличается от заявленного в профиле - будет эксцепшн. Сделано для проверки, чтобы не забыть поменять конструктор, если профиль изменился
//Result - указатель на объект движка. Врядл может понадобится, но почему бы не вернуть. Вдруг пригодится.
typedef cEngineInteractiveObject (__cdecl * PECREATEINTERACTIVEOBJECT)(int ID, void** SetAttributes, void* FillMask, int Size, vec3* Position, quat* Rotation);
//Генерирует событие удаления объекта. Объект удаляется не сразу, а в следующем кадре
typedef void (__cdecl * PEMARKOBJECTTODELETE) (cEngineInteractiveObject Object); 
//Загрузить уровень. Грузится не мгновенно, а в определенный момент. Поэтому функцию можно дергать в любой момент. Фактическая загрузка произойдет в течении следующего кадра.
typedef void (__cdecl * PELOADLEVELPROC) (const char* FileName); 
//Получение позиции камеры. -1 - основная камера. Остальные камеры могут задавать в редакторе
typedef void (__cdecl * PEGETCAMERAPROC) (int id, vec3* Position, vec3* Rotation);
//Установка позиции камеры. -1 - основня камера. Остальные камеры могут задаваться в редакторе уровней
typedef void (__cdecl * PESETCAMERAPROC) (int id, vec3* Position, vec3* Rotation);
//Загрузка модели. ГРузит и кэширует модели. Лучше использовать при загрузке уровня, а в реалтайме не дергать. Путь задается относительно каталога data/models
typedef cEngineModel (__cdecl * PEGETMODELPROC) (const char* Path, const char* Model);
//Устанавливаем количество слотов под модели в объекте. Все слоты нужно заполнить моделями до следующего кадра
typedef void (__cdecl *PESETMODELSCOUNTPROC) (cEngineInteractiveObject Object, int ModelsSlotsCount);
//Устанавливаем модель в слот.
typedef void (__cdecl *PESETMODELTPROC) (cEngineInteractiveObject Object, int ModelSlot, cIOModel Model);
//Получаем высоту ландшафта в точке. Функция весьма быстрая
typedef float (__cdecl *PEGETHEIGHTPROC) (float x, float y, bool Accurate);
//Загрузка шрифта. Шрифт грузится движковый.
typedef cEngineFont (__cdecl *PEGETFONTPROC) (const char* Font);
//Вывод текста. Имеет смысл делать только в событии E_DRAWINTERFACE
typedef void (__cdecl *PETEXTOUTPROC) (cEngineFont Font, int x, int y, float r, float g, float b, float a, const char* Text);
//Получение аттрибутов объекта
typedef void*(__cdecl *PEGETOBJECTATTRIBUTESPROC) (cEngineInteractiveObject Object);
//Рейтрейс. Выбирает объект по лучу.
typedef cEngineInteractiveObject(__cdecl *PEGETINTERACTIVEOBJECTPROC)(vec3* Position, quat* Direction, bool approximately);
//Получение координаты мыши в мире. Проекция курсора мыши в мир
typedef void (__cdecl *PEGETMOUSE3DPROC)(vec3 *Position, vec3 *OnScreenPosition, vec3 *View);
//Получение инфы о ландшафте. Это для физики
typedef cEngineLandscapeInfo* (__cdecl *PEGETLANDSCAPEINFOPROC)();
//Получение геометрии ландшафта. Для физики
typedef bool (__cdecl *PEGETLANDSCAPEMESHPROC)(int x, int y, vec3** VertexList, int** IndexList, int* VertexCount, int* IndexCount);
//Получение частичной геометрии ландшафта. Для физики
typedef bool (__cdecl *PEGETLANDSCAPEZONEMESHPROC)(float x, float y, float width, float height, cEngineLandscapeVertex*** VertexList, cEngineLandscapePoly*** PolyList, int* PolyCount);
//Установка позиции объекта в мире
typedef void (__cdecl *PESETOBJECTPOSITIONPROC)(cEngineInteractiveObject Object, vec3* Position);
//Установка ориентации объекта в мире кватернионом
typedef void (__cdecl *PESETOBJECTROTATIONPROC)(cEngineInteractiveObject Object, quat* Rotation);
//Установка ориентации объекта в мире матрицей
typedef void (__cdecl *PESETOBJECTROTATIONMATRIXPROC)(cEngineInteractiveObject Object, mat3x3* Rotation);
//Отрисовка отладочной линии. Доступно только в экспортируемом методе __declspec(dllexport) void __cdecl drawDebug()
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