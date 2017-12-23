#include "Input.h"


Input::Input()
{
	//初始化
	m_DI = NULL;
	m_Mouse = NULL;
	m_Keyboard = NULL;

	ZeroMemory(m_keyBuffer,sizeof(char)*256);
	ZeroMemory(&m_MouseState,sizeof(m_MouseState));

}


Input::~Input(void)
{
	if(m_Keyboard != NULL)
		m_Keyboard->Unacquire();
	if(m_Mouse != NULL)
		m_Mouse->Unacquire();
	SAFE_RELEASE(m_Keyboard);
	SAFE_RELEASE(m_Mouse);
	SAFE_RELEASE(m_DI);
}

HRESULT Input::Init(HWND l_hwnd)
{
	HRESULT hr;

	// 初始化DirectInput设备
	if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
		IID_IDirectInput8, ( VOID** )&m_DI, NULL ) ) )
		return hr;

	//初始化鼠标
	if( FAILED( m_DI->CreateDevice( GUID_SysMouse, &m_Mouse, NULL ) ) )
	{
		MessageBox( NULL, TEXT( "电脑上没有鼠标，程序即将结束。" ),
			TEXT( "桃源2D" ),
			MB_ICONERROR | MB_OK );
		return S_OK;
	}

	if( FAILED( hr = m_Mouse->SetDataFormat( &c_dfDIMouse ) ) )
		return hr;

	if( FAILED( hr = m_Mouse->SetCooperativeLevel( l_hwnd, DISCL_NONEXCLUSIVE |
		DISCL_FOREGROUND ) ) )
		return hr;

	//初始化键盘
	if( FAILED( m_DI->CreateDevice( GUID_SysKeyboard, &m_Keyboard, NULL ) ) )
	{
		MessageBox( NULL, TEXT( "电脑上没有键盘，程序即将结束。" ),
			TEXT( "桃源2D" ),
			MB_ICONERROR | MB_OK );
		return S_OK;
	}

	if( FAILED( hr = m_Keyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
		return hr;

	if( FAILED( hr = m_Mouse->SetCooperativeLevel( l_hwnd, DISCL_NONEXCLUSIVE |
		DISCL_FOREGROUND ) ) )
		return hr;

	return S_OK;
}

void Input::UpData()
{
	m_Keyboard->Poll();
	HRESULT hr = m_Keyboard->GetDeviceState(sizeof(m_keyBuffer), (void**)&m_keyBuffer); 
	//获取键盘输入消息
	if(hr)
	{
		m_Keyboard->Acquire();  
		m_Keyboard->GetDeviceState( sizeof(m_keyBuffer),(LPVOID)m_keyBuffer );
	}

	m_Keyboard->Poll();
	hr = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState); 
	//获取鼠标输入消息
	if(hr)
	{
		m_Mouse->Acquire();
		m_Mouse->GetDeviceState( sizeof(DIMOUSESTATE), (void**)&m_MouseState);
	}

}

bool Input::IsKeyDown(int l_key)
{
	if(m_keyBuffer[l_key] & 0x80)
		return true;
	else
		return false;
}

bool Input::IsMouseDown(int l_buttom)
{
	return (m_MouseState.rgbButtons[l_buttom] & 0x80) != 0;
}

float Input::MouseX()
{
	return (float)m_MouseState.lX;
}

//-----------------------------------------------------------------------------
// Name：DInputClass::MouseDY
// Desc: 返回鼠标指针的Y轴坐标值
//-----------------------------------------------------------------------------
float Input::MouseY()
{
	return (float)m_MouseState.lY;
}

//-----------------------------------------------------------------------------
// Name：DInputClass::MouseDZ
// Desc: 返回鼠标指针的Z轴坐标值（滚轮）
//-----------------------------------------------------------------------------
float Input::MouseZ()
{
	return (float)m_MouseState.lZ;
}