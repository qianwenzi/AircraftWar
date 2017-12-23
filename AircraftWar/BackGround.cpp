#include "BackGround.h"


BackGround::BackGround(IDirect3DDevice9 *_device)
{
	y = 0.0f;
	x = 0.0f;
	m_textureNum = TextureNum::T_Background;
	isBump = false;

	HRESULT hr;

	hr = _device->CreateVertexBuffer(sizeof(Vertex) *4, D3DUSAGE_WRITEONLY, FVF,D3DPOOL_MANAGED,&m_VB,NULL);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"创建m_VB错误",L"错误",NULL);

	}

	Vertex *pVertices = NULL;

	m_VB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = Vertex( -10.0f,  20.0f, 10.0f, 0.0f, 0.0f); //左上
	pVertices[1] = Vertex( 10.0f,  20.0f, 10.0f, 1.0f, 0.0f); //右上
	pVertices[2] = Vertex( 10.0f, -20.0f, 10.0f, 1.0f, 1.0f); //右下
	pVertices[3] = Vertex( -10.0f, -20.0f, 10.0f, 0.0f, 1.0f); //左下

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


BackGround::~BackGround(void)
{
}

void BackGround::Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device, ObjectManager *objManager)
{
	D3DXMatrixTranslation(&m_worldMat, x, y, 0);
}