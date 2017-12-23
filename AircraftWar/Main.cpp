//======================================头文件包含区域=====================================

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
//=========================================库文件包含区域================================================

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")     // 使用DirectInput必须包含的头文件，注意这里有8
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib") 

//=================================宏定义区域===============================================

#define WINDOW_WIDTH	618							//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	1000							//为窗口高度定义的宏，以方便在此处修改窗口高度
#define WINDOW_TITLE	L"飞机大战"	//为窗口标题定义的宏


//===================================全局变量声明区域=====================================
IDirect3DDevice9 *g_Device = NULL; //Direct3D设备对象
D3DXMATRIX g_matWorld;
Input *g_pDInput = NULL;
Camera *g_pCamera = NULL;
IDirect3DVertexBuffer9 * g_VB = NULL;
IDirect3DIndexBuffer9 *g_IB = NULL;
ObjectManager *g_ObjectManager = NULL;
Player *g_player = NULL;

float g_deltaTime;
float g_enemyCreateTime = 1500.0f;

ID3DXFont*								g_pFont=NULL;    //字体COM接口
float											g_FPS = 0.0f;       //一个浮点型的变量，代表帧速率
wchar_t										g_strFPS[100];    //包含帧速率的字符数组
wchar_t g_strPlayer[100];
//=====================================全局函数声明区域=======================================

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );//窗口过程函数
HRESULT Direct3D_Init(HWND hwnd);		 //在这个函数中进行Direct3D的初始化
HRESULT	Objects_Init(HWND hwnd); 		//在这个函数中进行要绘制的物体的资源初始化
VOID Direct3D_Render(HWND hwnd); 	//在这个函数中进行Direct3D渲染代码的书写
VOID Direct3D_CleanUp( );				//在这个函数中清理COM资源以及其他资源
void Direct3D_Update( HWND hwnd);

float							Get_FPS();								//计算帧数的函数

//=====================================入口函数====================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	//设计的窗口类
	WNDCLASSEX wndClass = { 0 };							//用WINDCLASSEX定义了一个窗口类
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;			//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;					//设置指向窗口过程函数的指针
	wndClass.cbClsExtra		= 0;								//窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra		= 0;							//窗口的附加内存，依然取0就行了
	wndClass.hInstance = hInstance;						//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //本地加载自定义ico图标
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //指定窗口类的光标句柄。
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  //为hbrBackground成员指定一个白色画刷句柄	
	wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = L"飞机大战";		//用一个以空终止的字符串，指定窗口类的名字。

	//注册窗口类
	if( !RegisterClassEx( &wndClass ) )				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		return -1;		

	//创建窗口
	HWND hwnd = CreateWindow( L"飞机大战",WINDOW_TITLE,				//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );

	//Direct3D资源的初始化，调用失败用messagebox予以显示
	if (!(S_OK==Direct3D_Init (hwnd)))
	{
		MessageBox(hwnd, _T("Direct3D初始化失败~！"), _T("浅墨的消息窗口"), 0); //使用MessageBox函数，创建一个消息窗口 
	}

	//窗口的移动、显示与更新
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
	ShowWindow( hwnd, nShowCmd );    //调用ShowWindow函数来显示窗口
	UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样

	g_pDInput = new Input();
	g_pDInput->Init(hwnd);

	//消息循环
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

			Direct3D_Render(hwnd);   //渲染
			Direct3D_Update(hwnd);

			g_deltaTime = ( (float)timeGetTime() - startTime) /1000.0f;

		}
	}
	//注销窗口类
	UnregisterClass(L"飞机大战", wndClass.hInstance);  
	return 0;  
}

//==================================窗口消息函数============================================

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )      
{
	switch( message )						//switch语句开始
	{
	case WM_PAINT:						
		Direct3D_Render(hwnd);               
		ValidateRect(hwnd, NULL);		// 更新客户区的显示
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
		return DefWindowProc( hwnd, message, wParam, lParam );		//调用缺省的窗口过程
	}

	return 0;									//正常退出
}


HRESULT Direct3D_Init(HWND hwnd)
{

	LPDIRECT3D9  pD3D = NULL; //创建Direct3D接口对象
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //初始化Direct3D接口对象，并进行DirectX版本协商
		return E_FAIL;


	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
	{
		return E_FAIL;
	}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算


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


	//创建Direct3D设备接口
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_Device)))
		return E_FAIL;

	SAFE_RELEASE(pD3D) //释放LPDIRECT3D9接口对象

		if(!(S_OK==Objects_Init(hwnd))) return E_FAIL;     //初始化渲染资源

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
	srand(timeGetTime());      //用系统时间初始化随机种子 

	D3DXCreateFont(g_Device, 36, 0, 0, 1, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("微软雅黑"), &g_pFont);



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
	MessageBox(NULL,L"创建纹理错误",L"错误",NULL);
	return E_FAIL;
	}
	break;

	case TextureNum::Enemy0:
	hr = D3DXCreateTextureFromFile(g_Device,L"",&g_texture[i]);
	if (hr != S_OK)
	{
	MessageBox(NULL,L"创建纹理错误",L"错误",NULL);
	return E_FAIL;
	}
	break;

	case TextureNum::Enemy1:
	hr = D3DXCreateTextureFromFile(g_Device,L"",&g_texture[i]);
	if (hr != S_OK)
	{
	MessageBox(NULL,L"创建纹理错误",L"错误",NULL);
	return E_FAIL;
	}
	break;
	}*/



	g_pCamera = new Camera(Camera::LANDOBJECT);
	g_pCamera->setPosition(&D3DXVECTOR3(0.0f, 0.0f, -25.0));  //设置摄像机所在的位置

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

	//获取主窗口矩形
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
		swprintf_s(g_strPlayer, L"血量为：%0.1f \n 击杀数：%0.1f", g_player->m_MP,g_player->JiSha );
		g_pFont->DrawText(NULL, g_strPlayer, -1 , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(255,39,136));
	}
	else
	{
		g_pFont->DrawText(NULL, L"你死了！请按esc键退出。", -1 , &formatRect, DT_CENTER , D3DCOLOR_XRGB(255,39,136));
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
	//计算并设置取景变换矩阵
	D3DXMATRIX matView;
	g_pCamera->getViewMatrix(&matView);
	g_Device->SetTransform(D3DTS_VIEW, &matView);

	//把正确的世界变换矩阵存到g_matWorld中
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, 0.0f);

	g_ObjectManager->Updata(g_pDInput,g_deltaTime,g_Device);

	////以下这段代码用于限制鼠标光标移动区域
	//POINT lt,rb;
	//RECT rect;
	//GetClientRect(hwnd,&rect);  //取得窗口内部矩形
	////将矩形左上点坐标存入lt中
	//lt.x = rect.left;
	//lt.y = rect.top;
	////将矩形右下坐标存入rb中
	//rb.x = rect.right;
	//rb.y = rect.bottom;
	////将lt和rb的窗口坐标转换为屏幕坐标
	//ClientToScreen(hwnd,&lt);
	//ClientToScreen(hwnd,&rb);
	////以屏幕坐标重新设定矩形区域
	//rect.left = lt.x;
	//rect.top = lt.y;
	//rect.right = rb.x;
	//rect.bottom = rb.y;
	////限制鼠标光标移动区域
	//ClipCursor(&rect);

	//ShowCursor(false);		//隐藏鼠标光标


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
	//定义四个静态变量
	static float  fps = 0; //我们需要计算的FPS值
	static int     frameCount = 0;//帧数
	static float  currentTime =0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间

	frameCount++;//每调用一次Get_FPS()函数，帧数自增1
	currentTime = timeGetTime()*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间

	//如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if(currentTime - lastTime > 1.0f) //将时间控制在1秒钟
	{
		fps = (float)frameCount /(currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
		frameCount    = 0;//将本次帧数frameCount值清零
	}

	return fps;
}