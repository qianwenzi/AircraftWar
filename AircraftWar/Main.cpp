//======================================ͷ�ļ���������=====================================

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include "Input.h"
#include "Camera.h"
#include "Object.h"
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "ObjectManager.h"
#include "BackGround.h"
//=========================================���ļ���������================================================

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")     // ʹ��DirectInput���������ͷ�ļ���ע��������8
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib") 

//=================================�궨������===============================================

#define WINDOW_WIDTH	618							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	1000							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	L"�ɻ���ս"	//Ϊ���ڱ��ⶨ��ĺ�


//===================================ȫ�ֱ�����������=====================================
IDirect3DDevice9 *g_Device = NULL; //Direct3D�豸����
D3DXMATRIX g_matWorld;
Input *g_pDInput = NULL;
Camera *g_pCamera = NULL;
IDirect3DVertexBuffer9 * g_VB = NULL;
IDirect3DIndexBuffer9 *g_IB = NULL;
ObjectManager *g_ObjectManager = NULL;
Player *g_player = NULL;

float g_deltaTime;
float g_enemyCreateTime = 1500.0f;

ID3DXFont*								g_pFont=NULL;    //����COM�ӿ�
float											g_FPS = 0.0f;       //һ�������͵ı���������֡����
wchar_t										g_strFPS[100];    //����֡���ʵ��ַ�����
wchar_t g_strPlayer[100];
//=====================================ȫ�ֺ�����������=======================================

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );//���ڹ��̺���
HRESULT Direct3D_Init(HWND hwnd);		 //����������н���Direct3D�ĳ�ʼ��
HRESULT	Objects_Init(HWND hwnd); 		//����������н���Ҫ���Ƶ��������Դ��ʼ��
VOID Direct3D_Render(HWND hwnd); 	//����������н���Direct3D��Ⱦ�������д
VOID Direct3D_CleanUp( );				//���������������COM��Դ�Լ�������Դ
void Direct3D_Update( HWND hwnd);

float							Get_FPS();								//����֡���ĺ���

//=====================================��ں���====================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	//��ƵĴ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra		= 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra		= 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //ָ��������Ĺ������
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"�ɻ���ս";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	//ע�ᴰ����
	if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;		

	//��������
	HWND hwnd = CreateWindow( L"�ɻ���ս",WINDOW_TITLE,				//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );

	//Direct3D��Դ�ĳ�ʼ��������ʧ����messagebox������ʾ
	if (!(S_OK==Direct3D_Init (hwnd)))
	{
		MessageBox(hwnd, _T("Direct3D��ʼ��ʧ��~��"), _T("ǳī����Ϣ����"), 0); //ʹ��MessageBox����������һ����Ϣ���� 
	}

	//���ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow( hwnd, nShowCmd );    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	g_pDInput = new Input();
	g_pDInput->Init(hwnd);

	//��Ϣѭ��
	MSG msg = { 0 };

	static float startTime = (float)timeGetTime(); 
	while( msg.message != WM_QUIT )			
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   
		{
			TranslateMessage( &msg );		
			DispatchMessage( &msg );		
		}
		else
		{
			startTime = (float)timeGetTime(); 

			Direct3D_Render(hwnd);   //��Ⱦ
			Direct3D_Update(hwnd);

			g_deltaTime = ( (float)timeGetTime() - startTime) /1000.0f;

		}
	}
	//ע��������
	UnregisterClass(L"�ɻ���ս", wndClass.hInstance);  
	return 0;  
}

//==================================������Ϣ����============================================

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )      
{
	switch( message )						//switch��俪ʼ
	{
	case WM_PAINT:						
		Direct3D_Render(hwnd);               
		ValidateRect(hwnd, NULL);		// ���¿ͻ�������ʾ
		break;							

	case WM_KEYDOWN:				
		if (wParam == VK_ESCAPE)   
			DestroyWindow(hwnd);
		if (wParam == 'B')   
			g_enemyCreateTime = 10.0f;
		break;									

	case WM_DESTROY:					
		Direct3D_CleanUp();			
		PostQuitMessage( 0 );		
		break;							

	default:										
		return DefWindowProc( hwnd, message, wParam, lParam );		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}


HRESULT Direct3D_Init(HWND hwnd)
{

	LPDIRECT3D9  pD3D = NULL; //����Direct3D�ӿڶ���
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
		return E_FAIL;


	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
	{
		return E_FAIL;
	}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò��������������


	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = WINDOW_WIDTH;
	d3dpp.BackBufferHeight           = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;


	//����Direct3D�豸�ӿ�
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_Device)))
		return E_FAIL;

	SAFE_RELEASE(pD3D) //�ͷ�LPDIRECT3D9�ӿڶ���

		if(!(S_OK==Objects_Init(hwnd))) return E_FAIL;     //��ʼ����Ⱦ��Դ

	g_Device->SetFVF(FVF);

	//g_Device->SetRenderState( D3DRS_LIGHTING, FALSE );
	//g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  
	//g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); 
	//   g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); 
	//
	//g_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//g_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Device->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_Device->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
	g_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


	return S_OK;
}


HRESULT Objects_Init(HWND hwnd)
{
	srand(timeGetTime());      //��ϵͳʱ���ʼ��������� 

	D3DXCreateFont(g_Device, 36, 0, 0, 1, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("΢���ź�"), &g_pFont);



	g_ObjectManager = new ObjectManager(g_Device);


	/*g_texture[i] = NULL;
	}

	for(int i = 0 ; i<3 ; i++)
	{
	switch(rand() % 3)
	{
	case TextureNum::Player:
	hr = D3DXCreateTextureFromFile(g_Device,L"",&g_texture[i]);
	if (hr != S_OK)
	{
	MessageBox(NULL,L"�����������",L"����",NULL);
	return E_FAIL;
	}
	break;

	case TextureNum::Enemy0:
	hr = D3DXCreateTextureFromFile(g_Device,L"",&g_texture[i]);
	if (hr != S_OK)
	{
	MessageBox(NULL,L"�����������",L"����",NULL);
	return E_FAIL;
	}
	break;

	case TextureNum::Enemy1:
	hr = D3DXCreateTextureFromFile(g_Device,L"",&g_texture[i]);
	if (hr != S_OK)
	{
	MessageBox(NULL,L"�����������",L"����",NULL);
	return E_FAIL;
	}
	break;
	}*/



	g_pCamera = new Camera(Camera::LANDOBJECT);
	g_pCamera->setPosition(&D3DXVECTOR3(0.0f, 0.0f, -25.0));  //������������ڵ�λ��

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
		1.0f,
		1000.0f);
	g_Device->SetTransform(D3DTS_PROJECTION, &proj);




	Player *player = new Player(g_Device);
	g_player = player;
	g_ObjectManager->AddObject(player);

	BackGround *background = new BackGround(g_Device);
	g_ObjectManager->AddObject(background);

	return S_OK;
}

void Direct3D_Render(HWND hwnd)
{

	//��ȡ�����ھ���
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);


	g_Device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	g_Device->BeginScene();                 




	if(g_player->m_MP > 0.0f)
	{
		g_ObjectManager->Render(g_Device,&g_matWorld);


		swprintf_s(g_strFPS, L"FPS:%0.1f ", Get_FPS() );
		g_pFont->DrawText(NULL, g_strFPS, -1 , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(255,39,136));

		formatRect.top += 40;
		swprintf_s(g_strPlayer, L"Ѫ��Ϊ��%0.1f \n ��ɱ����%0.1f", g_player->m_MP,g_player->JiSha );
		g_pFont->DrawText(NULL, g_strPlayer, -1 , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(255,39,136));
	}
	else
	{
		g_pFont->DrawText(NULL, L"�����ˣ��밴esc���˳���", -1 , &formatRect, DT_CENTER , D3DCOLOR_XRGB(255,39,136));
	}



	g_Device->EndScene();                  

	g_Device->Present(NULL, NULL, NULL, NULL);  
}

void Direct3D_Update( HWND hwnd)
{


	g_pDInput->UpData();
	//if (g_pDInput->IsKeyDown(DIK_A)) { g_pCamera->strafe(-0.2f); /*g_playerX -= 0.2f;*/ }
	//if (g_pDInput->IsKeyDown(DIK_D))  {g_pCamera->strafe( 0.2f); /*g_playerX += 0.2f;*/}
	//if (g_pDInput->IsKeyDown(DIK_W)) {g_pCamera->walk( 0.2f); /*g_playerZ += 0.2f;*/ }
	//if (g_pDInput->IsKeyDown(DIK_S))  {g_pCamera->walk(-0.2f);/*g_playerZ -= 0.2f;*/}
	//if (g_pDInput->IsKeyDown(DIK_R))  {g_pCamera->fly( 0.2f); /*g_playerY += 0.2f;*/ }
	//if (g_pDInput->IsKeyDown(DIK_F))  {g_pCamera->fly(-0.2f); /*g_playerY -= 0.2f;*/}

	//g_pCamera->yaw(g_pDInput->MouseX() * 0.001f);

	//g_pCamera->pitch(g_pDInput->MouseY() * 0.001f);
	//���㲢����ȡ���任����
	D3DXMATRIX matView;
	g_pCamera->getViewMatrix(&matView);
	g_Device->SetTransform(D3DTS_VIEW, &matView);

	//����ȷ������任����浽g_matWorld��
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, 0.0f);

	g_ObjectManager->Updata(g_pDInput,g_deltaTime,g_Device);

	////������δ�����������������ƶ�����
	//POINT lt,rb;
	//RECT rect;
	//GetClientRect(hwnd,&rect);  //ȡ�ô����ڲ�����
	////���������ϵ��������lt��
	//lt.x = rect.left;
	//lt.y = rect.top;
	////�����������������rb��
	//rb.x = rect.right;
	//rb.y = rect.bottom;
	////��lt��rb�Ĵ�������ת��Ϊ��Ļ����
	//ClientToScreen(hwnd,&lt);
	//ClientToScreen(hwnd,&rb);
	////����Ļ���������趨��������
	//rect.left = lt.x;
	//rect.top = lt.y;
	//rect.right = rb.x;
	//rect.bottom = rb.y;
	////����������ƶ�����
	//ClipCursor(&rect);

	//ShowCursor(false);		//���������


	static float _startTime;
	static float _lastTime;
	_lastTime = (float)timeGetTime();

	if(_lastTime - _startTime > g_enemyCreateTime)
	{
		switch(rand() % 2)
		{
		case 0:
			{
				Enemy *enemy = new Enemy(TextureNum::T_Enemy1,g_Device,g_player);
				g_ObjectManager->AddObject(enemy);
				if(rand() % 2 == 0)
				{
					enemy->x = (float)(rand() % 6 * 1);
				}
				else
				{
					enemy->x = (float)(rand() % 6 * (-1));
				}
				break;
			}
		case 1:
			{
				Enemy *enemy = new Enemy(TextureNum::T_Enemy0,g_Device,g_player);
				g_ObjectManager->AddObject(enemy);
				if(rand() % 2 == 0)
				{
					enemy->x = (float)(rand() % 6 * 1);
				}
				else
				{
					enemy->x = (float)(rand() % 6 * (-1));
				}
				break;
			}
		}


		_startTime = _lastTime;
	}


}

void Direct3D_CleanUp()
{
	SAFE_RELEASE(g_Device);
	SAFE_RELEASE(g_pFont);

	delete g_ObjectManager;
	delete g_pCamera;
	delete g_pDInput;
	delete g_player;
}

float Get_FPS()
{
	//�����ĸ���̬����
	static float  fps = 0; //������Ҫ�����FPSֵ
	static int     frameCount = 0;//֡��
	static float  currentTime =0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��

	frameCount++;//ÿ����һ��Get_FPS()������֡������1
	currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��

	//�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if(currentTime - lastTime > 1.0f) //��ʱ�������1����
	{
		fps = (float)frameCount /(currentTime - lastTime);//������1���ӵ�FPSֵ
		lastTime = currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		frameCount    = 0;//������֡��frameCountֵ����
	}

	return fps;
}