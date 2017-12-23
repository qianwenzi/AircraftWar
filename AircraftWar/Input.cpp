#include "Input.h"


Input::Input()
{
	//��ʼ��
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

	// ��ʼ��DirectInput�豸
	if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
		IID_IDirectInput8, ( VOID** )&m_DI, NULL ) ) )
		return hr;

	//��ʼ�����
	if( FAILED( m_DI->CreateDevice( GUID_SysMouse, &m_Mouse, NULL ) ) )
	{
		MessageBox( NULL, TEXT( "������û����꣬���򼴽�������" ),
			TEXT( "��Դ2D" ),
			MB_ICONERROR | MB_OK );
		return S_OK;
	}

	if( FAILED( hr = m_Mouse->SetDataFormat( &c_dfDIMouse ) ) )
		return hr;

	if( FAILED( hr = m_Mouse->SetCooperativeLevel( l_hwnd, DISCL_NONEXCLUSIVE |
		DISCL_FOREGROUND ) ) )
		return hr;

	//��ʼ������
	if( FAILED( m_DI->CreateDevice( GUID_SysKeyboard, &m_Keyboard, NULL ) ) )
	{
		MessageBox( NULL, TEXT( "������û�м��̣����򼴽�������" ),
			TEXT( "��Դ2D" ),
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
	//��ȡ����������Ϣ
	if(hr)
	{
		m_Keyboard->Acquire();  
		m_Keyboard->GetDeviceState( sizeof(m_keyBuffer),(LPVOID)m_keyBuffer );
	}

	m_Keyboard->Poll();
	hr = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState); 
	//��ȡ���������Ϣ
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
// Name��DInputClass::MouseDY
// Desc: �������ָ���Y������ֵ
//-----------------------------------------------------------------------------
float Input::MouseY()
{
	return (float)m_MouseState.lY;
}

//-----------------------------------------------------------------------------
// Name��DInputClass::MouseDZ
// Desc: �������ָ���Z������ֵ�����֣�
//-----------------------------------------------------------------------------
float Input::MouseZ()
{
	return (float)m_MouseState.lZ;
}