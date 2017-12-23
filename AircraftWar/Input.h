#pragma once
#pragma warning( disable:4005 )

#include <dinput.h>

#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if((p) != NULL) { delete (p);      } }       //自定义一个SAFE_RELEASE()宏,便于指针资源的释放
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if((p) != NULL) { (p)->Release();  } }     //自定义一个SAFE_RELEASE()宏,便于COM资源的释放
#endif


class Input
{
public:
	Input();
	~Input(void);

	HRESULT Init(HWND l_hwnd);
	void UpData();

	bool IsKeyDown(int l_key);
	bool IsMouseDown(int l_buttom);
	float MouseX();   
	float MouseY();	
	float MouseZ();	

private:
	LPDIRECTINPUT8 m_DI;
	LPDIRECTINPUTDEVICE8 m_Mouse;
	LPDIRECTINPUTDEVICE8 m_Keyboard;
	char m_keyBuffer[256];
	DIMOUSESTATE m_MouseState;

	double m_x;
	double m_y;
};

