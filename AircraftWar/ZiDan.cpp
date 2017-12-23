#include "ZiDan.h"
#include "ObjectManager.h"

ZiDan::ZiDan(IDirect3DDevice9 *_device, float formWhich)
{
	y = 10.0f;
	x = 0.0f;
	m_MP = 1;
	m_shangHai = 1;
	m_v = 3;
	m_fromWhich = formWhich;
	m_textureNum = TextureNum::T_ZiDan;

	if(formWhich == 1.0f)
		m_ObjectType = ObjectType::OBJT_ZiDanFromPlayer;
	else
		m_ObjectType = ObjectType::OBJT_ZiDanFromEnemy;

	HRESULT hr;

	hr = _device->CreateVertexBuffer(sizeof(Vertex) *4, D3DUSAGE_WRITEONLY, FVF,D3DPOOL_MANAGED,&m_VB,NULL);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"����g_VB����",L"����",NULL);

	}

	Vertex *pVertices = NULL;

	m_VB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = Vertex( -0.1f,  0.2f, 0.0f, 0.0f, 0.0f); //����
	pVertices[1] = Vertex( 0.1f,  0.2f, 0.0f, 1.0f, 0.0f); //����
	pVertices[2] = Vertex( 0.1f, -0.2f, 0.0f, 1.0f, 1.0f); //����
	pVertices[3] = Vertex( -0.1f, -0.2f, 0.0f, 0.0f, 1.0f); //����

	m_VB->Unlock();

	hr = _device->CreateIndexBuffer(6 * sizeof(WORD), 0,D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, NULL);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"����g_IB����",L"����",NULL);

	}

	WORD *pIndices;

	m_IB->Lock(0, 0, (void**)&pIndices, 0);
	pIndices[0] = 0, pIndices[1] = 1, pIndices[2] = 2; // ������V0V1V2
	pIndices[3] = 0, pIndices[4] = 2, pIndices[5] = 3; // ������V1V3V2
	m_IB->Unlock();
}


ZiDan::~ZiDan(void)
{
}

void ZiDan::Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device, ObjectManager *objManager)
{
	m_bumpBox.xMax = x + 0.1f;
	m_bumpBox.xMin = x - 0.1f;
	m_bumpBox.yMax = y + 0.2f;
	m_bumpBox.yMin = y - 0.2f;

	y = y + m_v * deltaTime * m_fromWhich;

	if (y >= 15.0f || y <= -15.0f)
	{
		m_isAlive = false;
	}

	D3DXMatrixTranslation(&m_worldMat, x, y, 0);
}

void ZiDan::OnBump(ObjectType objType)
{
	m_isAlive = false;
}

void ZiDan::OnUnBump(ObjectType objType)
{
}