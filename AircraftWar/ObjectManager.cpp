#include "ObjectManager.h"
#include "ZiDan.h"

ObjectManager::ObjectManager(IDirect3DDevice9 *_device)
{
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(_device,L"Data/Player.png",&m_texture[TextureNum::T_Player]);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"创建纹理错误",L"错误",NULL);
		return;
	}
	hr = D3DXCreateTextureFromFile(_device,L"Data/Enemy0.png",&m_texture[TextureNum::T_Enemy0]);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"创建纹理错误",L"错误",NULL);
		return;
	}
	hr = D3DXCreateTextureFromFile(_device,L"Data/Enemy1.png",&m_texture[TextureNum::T_Enemy1]);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"创建纹理错误",L"错误",NULL);
		return;
	}
	hr = D3DXCreateTextureFromFile(_device,L"Data/ZiDan.png",&m_texture[TextureNum::T_ZiDan]);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"创建纹理错误",L"错误",NULL);
		return;
	}
	hr = D3DXCreateTextureFromFile(_device,L"Data/Background.jpg",&m_texture[TextureNum::T_Background]);
	if (hr != S_OK)
	{
		MessageBox(NULL,L"创建纹理错误",L"错误",NULL);
		return;
	}
}


ObjectManager::~ObjectManager(void)
{
	for(int i = 0; i < 5; i++)
	{
		if(m_texture)
			m_texture[i]->Release();
	}
}

void ObjectManager::AddObject(Object *obj)
{
	m_objectList.push_back(obj);
}

void ObjectManager::Render(IDirect3DDevice9 *_device , D3DXMATRIX *matWorld)
{
	D3DXMATRIX mat;





	for (UINT i = 0; i < m_objectList.size(); i++)
	{
		_device->SetStreamSource(0, m_objectList[i]->m_VB, 0, sizeof(Vertex));
		_device->SetIndices(m_objectList[i]->m_IB);

		mat = *matWorld *  m_objectList[i]->m_worldMat;
		_device->SetTexture(0,m_texture[m_objectList[i]->m_textureNum]);
		_device->SetTransform( D3DTS_WORLD,  &mat);
		_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	}

}

void ObjectManager::Updata(Input *_input, float deltaTime , IDirect3DDevice9 *_device)
{
	for (UINT i = 0; i < m_objectList.size(); i++)
	{
		m_objectList[i]->Updata(_input, deltaTime,_device,this);
	}

	for (UINT i = 0; i < m_objectList.size(); i++)
	{
		if(m_objectList[i]->m_isAlive == false)
		{
			delete m_objectList[i];
			m_objectList.erase(m_objectList.begin() + i);		
		}
	}


	for (UINT i = 0; i < m_objectList.size(); i++)
	{
		for (UINT j = 0; j < m_objectList.size(); j++)
		{
			//if(m_objectList[i]->m_bumpBox.yMin <= m_objectList[0]->m_bumpBox.yMax)
			//{
			//	if(  
			//		((m_objectList[i]->m_bumpBox.xMax <= m_objectList[0]->m_bumpBox.xMax && m_objectList[i]->m_bumpBox.xMax >= m_objectList[0]->m_bumpBox.xMin) 
			//		|| (m_objectList[i]->m_bumpBox.xMin >= m_objectList[0]->m_bumpBox.xMin && m_objectList[i]->m_bumpBox.xMin <= m_objectList[0]->m_bumpBox.xMax))
			//		&&
			//		(m_objectList[i]->m_bumpBox.yMax >= m_objectList[0]->m_bumpBox.yMin)
			//		)
			//	{
			//		m_objectList[0]->OnBump();
			//		m_objectList[i]->OnBump();
			//	}
			//	else
			//	{
			//		m_objectList[0]->OnUnBump();
			//		m_objectList[i]->OnUnBump();
			//	}
			//}

			//是否可碰撞
			if(m_objectList[i]->isBump && m_objectList[j]->isBump)
			{
				if(i != j )
				{

					if(m_objectList[i]->m_bumpBox.xMin <= m_objectList[j]->m_bumpBox.xMax && m_objectList[i]->m_bumpBox.yMin <= m_objectList[j]->m_bumpBox.yMax &&
						m_objectList[i]->m_bumpBox.xMax >= m_objectList[j]->m_bumpBox.xMin && m_objectList[i]->m_bumpBox.yMax >= m_objectList[j]->m_bumpBox.yMin)
					{
						m_objectList[j]->OnBump(m_objectList[i]->m_ObjectType);
						m_objectList[i]->OnBump(m_objectList[j]->m_ObjectType);
					}
					else
					{
						m_objectList[j]->OnUnBump(m_objectList[i]->m_ObjectType);
						m_objectList[i]->OnUnBump(m_objectList[j]->m_ObjectType);
					}
				}
			}
		}
	}


}