#pragma once
#include "engine/engine_options.h"
#ifdef PLATFORM_WIN32

#include <windows.h>
#include <string>

#include "util/math/mmath.h"

#define apWindowStdIcon LoadIcon(NULL, IDI_APPLICATION)          //����������� ������
#define apWindowStrCursor LoadCursor(NULL, IDC_ARROW)            //����������� ������
#define apWindowStrBackBrush (HBRUSH)GetStockObject(GRAY_BRUSH)  //����������� ���

struct apWindowsManager;

//������� ����
struct apWindow
{
	HWND mHWnd;                  //����
	fRect mRect;                 //������������� ����
	fRect mInRect;               //���������� ����� ����

	std::string mWndName;        //��� ����

	apWindowsManager *mpManager; //�������� ����

	int mPressedKeys[3];         //������� ������� ������

	bool mLeftMouseButton;       //����� ������ ����
	bool mRightMouseButton;      //������ ������ ����

	vec2 mCursorPos;             //������� �������

	bool mActiveWindow;          //������������ �� ����

	//������� �������������
	bool initialize(WNDPROC winProc, HICON icon, HCURSOR cursor, HBRUSH backBrush, std::string className,  std::string wndName, RECT rect);

	//�������������� �� WNDCLASSEX
	bool initialize(const WNDCLASSEX& wndClass, std::string wndName, RECT rect);

	//����������� �������������
	bool initializeStd(WNDPROC winProc, std::string wndName, RECT rect);

	//�������� ����
	void showWindow();

	//��������� ��������� ����
	LRESULT windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//���������:
	virtual void onMouseLeftButtonDown(vec2 point);
	virtual void onMouseLeftButtonUp(vec2 point);
	
	virtual void onMouseRightButtonDown(vec2 point);
	virtual void onMouseRightButtonUp(vec2 point);

	virtual void onMouseMove(vec2 point);
	virtual void onMouseWheel(float delta);
	
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

	virtual void onCreate(fRect inRect);
	virtual void onClose();
	virtual void onSize(fRect inRect);

	virtual void onActive();
	virtual void onDeActive();

	virtual float onTimer();
	//------------------------------//

	//������ �� ������ � ��������
	bool isKeyPressed(int key); 

	int pressedKey(int idx);
};

#endif