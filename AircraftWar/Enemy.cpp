#include "Enemy.h"
#include "ZiDan.h"
#include "ObjectManager.h"

Enemy::Enemy(int _type,IDirect3DDevice9 *_device, Player *_player)
{
	y = 10.0f;
	x = 0.0f;
	m_textureNum = _type;
	m_MP = 1;
	m_shangHai = 1;
	m_v = 1;
	m_player = _player;
	startTime = 0.0f;
	lastTime = 0.0f;
	m_ObjectType = ObjectType::OBJT_Enemy;

	HRESULT hr;

	hr = _device->CreateVertexBuffer(sizeof(Vertex) *4, D3DUSAGE_WRITEONLY, FVF,D3DPOOL_MANAGED,&m_VB,NULL);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"创建m_VB错误",L"错误",NULL);

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
		MessageBox(NULL,L"创建m_IB错误",L"错误",NULL);

	}

	WORD *pIndices = NULL;

	m_IB->Lock(0, 0, (void**)&pIndices, 0);
	pIndices[0] = 0, pIndices[1] = 1, pIndices[2] = 2; // 三角形V0V1V2
	pIndices[3] = 0, pIndices[4] = 2, pIndices[5] = 3; // 三角形V1V3V2
	m_IB->Unlock();
}	


Enemy::~Enemy(void)
{
}

void Enemy::Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device, ObjectManager *objManager)
{
	y -= m_v * deltaTime; 

	m_bumpBox.xMax = x + 1.0f;
	m_bumpBox.xMin = x - 1.0f;
	m_bumpBox.yMax = y + 1.0f;
	m_bumpBox.yMin = y - 1.0f;

	lastTime = (float)timeGetTime();

	if (y >= 15.0f || y <= -15.0f)
	{
		m_isAlive = false;
	}

	if(lastTime - startTime > 1000.0f)
	{	
		ZiDan *zidan = new ZiDan(_device,-1.0f);
		zidan->x = x;
		zidan->y = y - 1.5f;
		objManager->AddObject(zidan);

		startTime = lastTime;
	}


	if(x > m_player->x)
		x -= m_v * deltaTime;
	if(x < m_player->x)
		x += m_v * deltaTime;


	D3DXMatrixTranslation(&m_worldMat, x, y, 0);
}

void Enemy::OnBump(ObjectType objType)
{
	//这个函数碰撞会被调用两次
	if(objType != ObjectType::OBJT_ZiDanFromEnemy && objType != ObjectType::OBJT_Enemy)
	{
		m_player->JiSha += 0.5f;
		m_isAlive = false;
	}
}