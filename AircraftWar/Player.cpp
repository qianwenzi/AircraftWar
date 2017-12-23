#include "Player.h"
#include "ObjectManager.h"

Player::Player(IDirect3DDevice9 *_device)
{
	x = 0.0f;
	y = -8.0f;
	m_textureNum = TextureNum::T_Player;
	m_MP = 10.0f;
	m_v = 2.0f;
	m_shangHai = 1;
	JiSha = 0.0f;
	m_ObjectType = ObjectType::OBJT_Player;

	startTime = 0.0f;
	lastTime = 0.0f;

	HRESULT hr;

	hr = _device->CreateVertexBuffer(sizeof(Vertex) *4, D3DUSAGE_WRITEONLY, FVF,D3DPOOL_MANAGED,&m_VB,NULL);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"创建g_VB错误",L"错误",NULL);

	}

	Vertex *pVertices = NULL;

	m_VB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = Vertex( -1.0f,  1.0f, 0.0f, 0.0f, 0.0f); //左上
	pVertices[1] = Vertex( 1.0f,  1.0f, 0.0f, 1.0f, 0.0f); //右上
	pVertices[2] = Vertex( 1.0f, -1.0f, 0.0f, 1.0f, 1.0f); //右下
	pVertices[3] = Vertex( -1.0f, -1.0f, 0.0f, 0.0f, 1.0f); //左下

	m_VB->Unlock();

	hr = _device->CreateIndexBuffer(6 * sizeof(WORD), 0,D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, NULL);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"创建g_IB错误",L"错误",NULL);

	}

	WORD *pIndices = NULL;

	m_IB->Lock(0, 0, (void**)&pIndices, 0);
	pIndices[0] = 0, pIndices[1] = 1, pIndices[2] = 2; // 三角形V0V1V2
	pIndices[3] = 0, pIndices[4] = 2, pIndices[5] = 3; // 三角形V1V3V2
	m_IB->Unlock();
}


Player::~Player(void)
{
}


void Player::Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device, ObjectManager *objManager)
{	

	if (_input->IsKeyDown(DIK_A)) 
	{ 
		x = x -= m_v * deltaTime;
	}
	if (_input->IsKeyDown(DIK_D)) 
	{ 
		x = x += m_v * deltaTime;
	}
	if (_input->IsKeyDown(DIK_W)) 
	{ 
		y = y += m_v * deltaTime;
	}
	if (_input->IsKeyDown(DIK_S)) 
	{ 
		y = y -= m_v * deltaTime;
	}

	m_bumpBox.xMax = x + 1.0f;
	m_bumpBox.xMin = x - 1.0f;
	m_bumpBox.yMax = y + 1.0f;
	m_bumpBox.yMin = y - 1.0f;

	if (y >= 15.0f || y <= -15.0f)
	{
		m_isAlive = false;
	}

	lastTime = (float)timeGetTime();

	if(lastTime - startTime > 1000.0f)
	{	
		ZiDan *zidan = new ZiDan(_device,1.0f);
		zidan->x = this->x;
		zidan->y = this->y + 1.5f;
		objManager->AddObject(zidan);
		startTime = lastTime;
	}


	D3DXMatrixTranslation(&m_worldMat, x, y, 0);
}

void Player::OnBump(ObjectType objType)
{
	m_MP -= 0.5f;
}

void Player::OnUnBump(ObjectType objType)
{

}